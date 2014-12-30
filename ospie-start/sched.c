#include "sched.h"
#include "phyAlloc.h"
#include "hw.h"
#include "syscall.h"
#include "defines.h"
#include <stdint.h>

void funct_idle(){
	while(1);
}


void init_sched(){
	IDLE = phyAlloc_alloc(sizeof(pcb_s));
	init_pcb(IDLE, funct_idle, NULL, STACK_SIZE, NORMAL);
	current_blocked = NULL;
	
	scheduler_function = sched_round_robin;
	//scheduler_function = sched_fixed_priority;

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
	aPCB->stack_base= (unsigned int)phyAlloc_alloc(stackSize);

	uint32_t* sp = (uint32_t*) (aPCB->stack_base + stackSize);
	sp--;
	*sp = 0x13;
	sp--;
	*sp = (uint32_t) &start_current_process;
	sp -= 14;
	ctx->sp = sp;
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
	ctx_switch();
}

void elect(){
	//Selection of a new process to run
	current_process = scheduler();

	if(current_process->state==READY){
		current_process->state=RUNNING;
	}
}


pcb_s* scheduler(){
	return scheduler_function();
}

pcb_s* sched_round_robin(){
	pcb_s* current = queue_round_robin->first;
	pcb_s* tmp;
	//if process not alone
	if(current->next != current) {
		//Suppressing all terminated process that are not the current one
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
			//Handling waiting processes
			else if(current->state == WAITING){
				if(current->sleepingTime>0){
					(current->sleepingTime)--;
				}else{
					current->state=READY;
				}
			}
			current = current->next;
		} while(current != queue_round_robin->first);
		if(current_process == IDLE){
			return current;
		}else{
			return current_process->next;
		}
		//if process is alone
	} else {
		// if process can't be run
		if(current->state == TERMINATED || current->state == WAITING){
			return IDLE;
		}else{
			return current;
		}
	}
}

/*Function that suppresses all the terminated processes for the priority scheduler
*/
void cleanTerminated(){
	int i;
	//for each queues
	for(i = 0; i<PRIORITY_NUM; ++i){
		pcb_s* process_it = queue_fixed_priority[i]->first;
		//if queue not empty
		if(process_it != NULL){
			do{
				//Suppressing all terminated process that are not the current one
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
				//Handling waiting processes
				else if(process_it->state == WAITING){
					if(process_it->sleepingTime>0){
						(process_it->sleepingTime)--;
					}else{
						process_it->state=READY;
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
	//for each queue starting from the HIGHEST priority one
	for(i= PRIORITY_NUM-1; i>=0; --i){
		//if queue is not empty
		if(queue_fixed_priority[i]->first != NULL){
			pcb_s* it = queue_fixed_priority[i]->first;
			pcb_s* first = it;
			//if it's the current process' queue (and it's not the idle process)
			if(current_process != IDLE && i == current_process->priority){
				// if the current process is alone and can't be run
				if(current_process == current_process->next && (current_process->state == TERMINATED || current_process->state == WAITING)){
					continue;
				}
				//we start from the current process
				else{
					first = current_process;
					it = current_process->next;
				}
			}
			//we check the whole queue and return the first process that can be run
			do{
				if(it->state != TERMINATED && it->state != WAITING){
					return it;
				}
				it = it->next;
			}while(it != first);
		}
	}
	//if no runnable process can be found -> we return IDLE
	return IDLE;
}

void start_sched(){
	current_process=IDLE;
	ENABLE_IRQ();
	set_tick_and_enable_timer_with_time(INTERRUPT_TIME(current_process->priority));
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

	set_tick_and_enable_timer_with_time(INTERRUPT_TIME(current_process->priority));

	__asm("pop {r0-r12, lr}");

	ENABLE_IRQ();	
	 
	__asm("rfeia sp!");	
}

void __attribute__ ((naked)) ctx_switch(){

	DISABLE_IRQ();

	//1. sauvegarde le contexte du processus en cours d’exécution
	__asm("srsdb sp!, #0x13");
	__asm("push {r0-r12, lr}");
	__asm("mov %0, sp" : "=r"(current_process->ctx->sp));	

	//2. demande au scheduler d’élire un nouveau processus
	elect();

	//3. restaure le contexte du processus élu
	__asm("mov sp, %0" : : "r"(current_process->ctx->sp));	

	set_tick_and_enable_timer_with_time(INTERRUPT_TIME(current_process->priority));

	__asm("pop {r0-r12, lr}");
	
	ENABLE_IRQ();

	__asm("rfeia sp!");	
}


void __attribute__ ((naked)) ctx_switch_from_wait(){
	
	unsigned int nbQuantums;
	__asm("mov %0, r1" : "=r"(nbQuantums));
	
	current_process->state = WAITING;
	current_process->sleepingTime = nbQuantums;	
	
	//2. demande au scheduler d’élire un nouveau processus
	elect();

	//3. restaure le contexte du processus élu
	__asm("mov sp, %0" : : "r"(current_process->ctx->sp));	

	set_tick_and_enable_timer_with_time(INTERRUPT_TIME(current_process->priority));

	__asm("pop {r0-r12, lr}");
	
	ENABLE_IRQ();

	__asm("rfeia sp!");
	
}

pcb_s* get_current_process() {
	return current_process;
}

void elect_blocked_process() {
	if(current_blocked != NULL) {
		current_blocked->process->state = READY;
		if(current_blocked->next = current_blocked) {
			phyAlloc_free(current_blocked, sizeof(blocked_process));
			current_blocked = NULL;
		} else {
			blocked_process* save = current_blocked;
			current_blocked->next->previous = current_blocked->previous;
			current_blocked->previous->next = current_blocked->next;
			current_blocked = current_blocked->next;
			phyAlloc_free(save, sizeof(blocked_process));
		}
	}
}

void add_blocked_process(sem_s* sem) {
	if(sem == NULL) return;
	blocked_process* new_blocked_process = phyAlloc_alloc(sizeof(blocked_process));
	new_blocked_process->sem = sem;
	new_blocked_process->process = current_process;
	current_blocked->previous = new_blocked_process;
	new_blocked_process->next = current_blocked;
	current_blocked->process->state = BLOCKED;
}
