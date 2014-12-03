#include "sched.h"
#include "phyAlloc.h"
#include "hw.h"

void funct_idle(){
	while(1);
}

void init_ctx(struct ctx_s* ctx, unsigned int stack_size){
        ctx->sp = (unsigned int)phyAlloc_alloc(stack_size) + stack_size - 14*4;
        ctx->lr = (unsigned int) start_current_process;
}

void init_sched(){

	init_pcb(IDLE, funct_idle, NULL, STACK_SIZE, NORMAL);

	scheduler_function = sched_fixed_priority;

	if(scheduler_function == sched_round_robin){
		queue_round_robin->first = NULL;
	}
	else if(scheduler_function == sched_fixed_priority){
		int i=0;
		for(i; i<PRIORITY_NUM; ++i){
			queue_fixed_priority[i] = phyAlloc_alloc(sizeof(queue));
			queue_fixed_priority[i]->first = NULL;
		}
	}
}

void init_pcb(pcb_s* aPCB, func_t f, void* args, unsigned int stackSize, Priority priority){
	
	aPCB->state = NEW;
	aPCB->function = f;
	aPCB->functionArgs = args;
	aPCB->priority = priority;

	ctx_s* ctx = phyAlloc_alloc(sizeof(ctx_s));
	init_ctx(ctx, stackSize);
	aPCB->ctx = ctx;

	aPCB->stack_size = stackSize;
}

void create_process(func_t f, void* args, unsigned int stack_size, Priority priority){
	DISABLE_IRQ();

	pcb_s* pcb = phyAlloc_alloc(sizeof(pcb_s));

	init_pcb(pcb, f, args, stack_size, priority);

	if(scheduler_function == sched_round_robin){
		add_pcb(queue_round_robin,pcb);
	}
	else if(scheduler_function == sched_fixed_priority){
		add_pcb(queue_fixed_priority[priority], pcb);
	}
	set_tick_and_enable_timer();
	ENABLE_IRQ();
}

void add_pcb(queue* queue, pcb_s* pcb){
	if (queue->first == NULL){
		queue->first = pcb;
		queue->last = pcb;
		pcb->next=pcb;
		pcb->previous=pcb;
	}
	else{
		pcb->previous=queue->last;
		queue->last->next = pcb;
		queue->last = pcb;
		queue->last->next = queue->first;
		queue->first->previous= queue->last;
	}
}

void start_current_process(){
	current_process->state = RUNNING;
	current_process->function(current_process->functionArgs);
	
	current_process->state = TERMINATED;
	//FIXME : crap -> can be interrupted
	ctx_switch();
}

void elect(){
	current_process = scheduler();
}

pcb_s* scheduler(){
	return scheduler_function();
}

pcb_s* sched_round_robin(){
	//TODO : only choose a READY proccess 
	// if no processes found -> run IDLE process (has to be created at init)

	current_process = current_process->next;

	//terminaison
	//TODO : add IDLE process when every process is terminated
	while(current_process->state == TERMINATED){
		pcb_s* tmp_process = current_process->next;

		//TODO : first and last of queue need to be updated
		current_process->previous->next = current_process->next;
		current_process->next->previous = current_process->previous;

		phyAlloc_free((void*)current_process->ctx->sp, current_process->stack_size);
		phyAlloc_free(current_process->ctx, sizeof(ctx_s));
		phyAlloc_free(current_process, sizeof(pcb_s));
	
		current_process = tmp_process;
	}
	return current_process;
}

void cleanTerminated(){

	int i = 0;
	for(i; i<PRIORITY_NUM; ++i){
		pcb_s* process_it = queue_fixed_priority[i]->first;
		if(process_it != NULL){
			do{
				if(process_it->state == TERMINATED && current_process != process_it){
					
					pcb_s* tmp;

					if(process_it->next == process_it){
						queue_fixed_priority[i]->first = NULL;
						process_it->previous = NULL;

						phyAlloc_free((void*)process_it->ctx->sp, process_it->stack_size);
		 				phyAlloc_free(process_it->ctx, sizeof(ctx_s));
		 				phyAlloc_free(process_it, sizeof(pcb_s));

		 				break;
					}
					else {
						if(queue_fixed_priority[i]->first == process_it){
		 					queue_fixed_priority[i]->first = process_it->next;
		 				}
		 				else if(queue_fixed_priority[i]->last == process_it){
		 					queue_fixed_priority[i]->last = process_it->previous;
		 				}
		 				process_it->previous->next = process_it->next;
		 				process_it->next->previous = process_it->previous;

		 				tmp = process_it->previous;

		 				phyAlloc_free((void*)process_it->ctx->sp, process_it->stack_size);
		 				phyAlloc_free(process_it->ctx, sizeof(ctx_s));
		 				phyAlloc_free(process_it, sizeof(pcb_s));

	 					process_it = tmp;
	 				}
				}
				if(process_it != NULL){
					process_it = process_it->next;
				}
			}while(process_it != queue_fixed_priority[i]->first);
		}
	}

	// int i = 0;
	// for(i; i<PRIORITY_NUM; ++i){
	// 	pcb_s* process = queue_fixed_priority[i]->first;
	// 	if(process != NULL){
	// 		if(process->next == process){
	// 			queue_fixed_priority[i]->first = NULL;
	// 		}
	// 		do{
	// 			if(process->state == TERMINATED && process != current_process){
	// 				pcb_s* tmp_process = process->next;

	// 				if(queue_fixed_priority[i]->first == process){
	// 					queue_fixed_priority[i]->first = process->next;
	// 				}
	// 				else if(queue_fixed_priority[i]->last == process){
	// 					queue_fixed_priority[i]->last = process->previous;
	// 				}
	// 				process->previous->next = process->next;
	// 				process->next->previous = process->previous;

	// 				phyAlloc_free((void*)process->ctx->sp, process->stack_size);
	// 				phyAlloc_free(process->ctx, sizeof(ctx_s));
	// 				phyAlloc_free(process, sizeof(pcb_s));

	// 				process = tmp_process;
	// 			}else{
	// 				process = process->next;
	// 			}
	// 		}while(process != queue_fixed_priority[i]->first);
	// 	}
	// }
}

pcb_s* sched_fixed_priority(){

	cleanTerminated();

	int i = PRIORITY_NUM-1;
	for(i; i>=0; --i){
		if(queue_fixed_priority[i]->first != NULL){
			if(i == current_process->priority && current_process != IDLE){
				if(current_process == current_process->next && current_process->state == TERMINATED){
					continue;
				}
				return current_process->next;
			}
			return queue_fixed_priority[i]->first;
		}
	}
	return IDLE;

	// // TODO : handle state
	// int i = PRIORITY_NUM-1;
	// int j = current_process->priority;
	// pcb_s* first;
	// pcb_s* current;
	// for(i; i>j; --i){
	// 	if(queue_fixed_priority[i]->first != NULL){
	// 		first = queue_fixed_priority[i]->first;
	// 		current = first;
	// 		while(current->state == TERMINATED){
	// 			current = current->next;
	// 			if(current == first) break;
	// 		}
	// 		if(current->state != TERMINATED){
	// 			return current;
	// 		}
	// 	}
	// }
	// pcb_s* old = current_process;
	// current = current_process->next;
	// while(current->state == TERMINATED && current != old) {
	// 	current = current->next;
	// }

	// if(current == old && current->state == TERMINATED) {
	// 	for(i = i - 1; i >= 0; --i) {
	// 		if(queue_fixed_priority[i]->first != NULL){
	// 			first = queue_fixed_priority[i]->first;
	// 			current = first;
	// 			while(current->state == TERMINATED){
	// 				current = current->next;
	// 				if(current == first) break;
	// 			}
	// 			if(current->state != TERMINATED){
	// 				return current;
	// 			}
	// 		}
	// 	}
	// 	return IDLE;
	// }
	// return current;
}

void start_sched(){
	current_process=IDLE;
	ENABLE_IRQ();
	set_tick_and_enable_timer();
}

void __attribute__ ((naked)) ctx_switch_from_irq(){
	
	DISABLE_IRQ();

	__asm("sub lr, lr, #4");
	__asm("srsdb sp!, #0x13");
	__asm("cps #0x13");

	__asm("push {r0-r12}");
	if(current_process->state == RUNNING){
		__asm("mov %0, sp" : "=r"(current_process->ctx->sp));	
		__asm("mov %0, lr" : "=r"(current_process->ctx->lr));
	}

	//2. demande au scheduler d’élire un nouveau processus
	elect();

	//3. restaure le contexte du processus élu
	__asm("mov sp, %0" : : "r"(current_process->ctx->sp));	
	__asm("mov lr, %0" : : "r"(current_process->ctx->lr));

	__asm("pop {r0-r12}");

	set_tick_and_enable_timer();
	ENABLE_IRQ();	
	
	if(current_process->state == NEW){
		start_current_process();	
	}
	__asm("rfeia sp!");	
}

void __attribute__ ((naked)) ctx_switch(){

	//1. sauvegarde le contexte du processus en cours d’exécution
	__asm("push {r0-r12}");	
	
	if(current_process->state == RUNNING){
		__asm("mov %0, sp" : "=r"(current_process->ctx->sp));	
		__asm("mov %0, lr" : "=r"(current_process->ctx->lr));
	}

	//2. demande au scheduler d’élire un nouveau processus
	elect();

	//3. restaure le contexte du processus élu
	__asm("mov sp, %0" : : "r"(current_process->ctx->sp));	
	__asm("mov lr, %0" : : "r"(current_process->ctx->lr));

	__asm("pop {r0-r12}");
	
	__asm("bx lr");
	
}
