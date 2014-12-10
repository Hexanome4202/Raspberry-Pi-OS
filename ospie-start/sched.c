#include "sched.h"
#include "phyAlloc.h"
#include "hw.h"
#include "syscall.h"
#include <stdint.h>

void funct_idle(){
	while(1);
}


void init_sched(){

	init_pcb(IDLE, funct_idle, NULL, STACK_SIZE, NORMAL);
	
	

	//scheduler_function = sched_round_robin;
	scheduler_function = sched_fixed_priority;

	if(scheduler_function == sched_round_robin){
		queue_round_robin->first = NULL;
	}
	else if(scheduler_function == sched_fixed_priority){
		int i;
		for(i=0; i<PRIORITY_NUM; ++i){
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
	aPCB->stack_base= (unsigned int)phyAlloc_alloc(stackSize) + stackSize - 14*4;
       // ctx->lr = (unsigned int) start_current_process;
	uint32_t* sp = (uint32_t*) (aPCB->stack_base + stackSize);
	sp--;
	*sp = 0x13;
	sp--;
	*sp = (uint32_t) &start_current_process;
	sp -= 14;
	ctx->sp = *sp;
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
	ctx_switch_from_irq();
}

void elect(){

	while(current_process->state==WAITING){
		
		if(current_process->sleepingTime>0){
			(current_process->sleepingTime)--;
		}else{
			current_process->state=READY;
		}
		if(current_process->next->state==WAITING){
			current_process = current_process->next;
		}
	}

	if(current_process->next->state==READY){
			current_process->next->state==RUNNING;
	}

	current_process = scheduler();
	
	

	//terminaison
	while(current_process->state == TERMINATED){
		pcb_s* tmp_process = current_process->next;

		current_process->previous->next = current_process->next;
		current_process->next->previous = current_process->previous;

		phyAlloc_free((void*)current_process->ctx->sp, current_process->stack_size);
		phyAlloc_free(current_process->ctx, sizeof(ctx_s));
		phyAlloc_free(current_process, sizeof(pcb_s));
	}
	
	//traiter cas ou le prochain process est waiting.. faire un autre if et passer de nouveau au next et decrementer sleepngTime
}

pcb_s* scheduler(){
	return scheduler_function();
}

pcb_s* sched_round_robin(){
	pcb_s* current = queue_round_robin->first;
	pcb_s* tmp;
	if(current->next != current) {
		do {
			if(current->state == TERMINATED && current != current_process) {
				if(queue_round_robin->first == current){
					queue_round_robin->first = current->next;
	 			}
	 			else if(queue_round_robin->last == current){
	 				queue_round_robin->last = current->previous;
	 			}
	 			current->previous->next = current->next;
	 			current->next->previous = current->previous;
	 			tmp = current->previous;

	 			phyAlloc_free((void*)current->ctx->sp, current->stack_size);
	 			phyAlloc_free(current->ctx, sizeof(ctx_s));
	 			phyAlloc_free(current, sizeof(pcb_s));

	 			current = tmp;
			}
			current = current->next;
		} while(current != queue_round_robin->first);
		if(current_process == IDLE){
			return current;
		}else{
			return current_process->next;
		}
	} else {
		if(current->state == TERMINATED){
			return IDLE;
		}else{
			return current;
		}
	}
}

void cleanTerminated(){

	int i;
	for(i = 0; i<PRIORITY_NUM; ++i){
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
}

pcb_s* sched_fixed_priority(){

	cleanTerminated();

	int i;
	for(i= PRIORITY_NUM-1; i>=0; --i){
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
}

void start_sched(){
	current_process=IDLE;
}

void __attribute__ ((naked)) ctx_switch_from_irq(){
	
	DISABLE_IRQ();

	__asm("sub lr, lr, #4");
	__asm("srsdb sp!, #0x13");
	__asm("cps #0x13");

	__asm("push {r0-r12, lr}");
	
	__asm("mov %0, sp" : "=r"(current_process->ctx->sp));

	//2. demande au scheduler d’élire un nouveau processus
	elect();

	//3. restaure le contexte du processus élu
	__asm("mov sp, %0" : : "r"(current_process->ctx->sp));	

	set_tick_and_enable_timer();
	
	__asm("pop {r0-r12, lr}");

	ENABLE_IRQ();	
	
	__asm("rfeia sp!");	
}

void __attribute__ ((naked)) ctx_switch(){

	DISABLE_IRQ();

	//1. sauvegarde le contexte du processus en cours d’exécution
	__asm("push {r0-r12}");	
	__asm("mov %0, sp" : "=r"(current_process->ctx->sp));	
	__asm("mov %0, lr" : "=r"(current_process->ctx->lr));

	//2. demande au scheduler d’élire un nouveau processus
	elect();

	//3. restaure le contexte du processus élu
	__asm("mov sp, %0" : : "r"(current_process->ctx->sp));	
	__asm("mov lr, %0" : : "r"(current_process->ctx->lr));

	__asm("pop {r0-r12}");
	
	set_tick_and_enable_timer();
	ENABLE_IRQ();

	__asm("bx lr");
	
}


void __attribute__ ((naked)) ctx_switch_from_wait(){
	
	DISABLE_IRQ();
	
	unsigned int nbQuantums;
	__asm("mov %0, r1" : "=r"(nbQuantums));
	
	current_process->state = WAITING;
	current_process->sleepingTime = nbQuantums;	
	
	ctx_switch_from_irq();
	
}
