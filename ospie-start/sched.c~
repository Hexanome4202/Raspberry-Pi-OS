#include "sched.h"
#include "phyAlloc.h"
#include "hw.h"


void init_ctx(struct ctx_s* ctx, unsigned int stack_size){
        ctx->sp = (unsigned int)phyAlloc_alloc(stack_size) + stack_size - 14*4;
        ctx->lr = (unsigned int) start_current_process;
}

void init_pcb(pcb_s* aPCB, func_t f, void* args, unsigned int stackSize){
	
	aPCB->state = NEW;
	aPCB->function = f;
	aPCB->functionArgs = args;

	ctx_s* ctx = phyAlloc_alloc(sizeof(ctx_s));
	init_ctx(ctx, stackSize);
	aPCB->ctx = ctx;

	aPCB->stack_size = stackSize;
}

void create_process(func_t f, void* args, unsigned int stack_size){
	DISABLE_IRQ();

	pcb_s* pcb = phyAlloc_alloc(sizeof(pcb_s));

	init_pcb(pcb, f, args, stack_size);

	if (first == NULL){
		first = pcb;
		last = pcb;
		pcb->next=pcb;
		pcb->previous=pcb;
	}
	else{
		pcb->previous=last;
		last->next = pcb;
		last = pcb;
		last->next = first;
		first->previous=last;
	}
	set_tick_and_enable_timer();
	ENABLE_IRQ();
}

void start_current_process(){
	current_process->state = RUNNING;
	current_process->function(current_process->functionArgs);
	
	current_process->state = TERMINATED;
	ctx_switch();
}

void elect(){
	current_process=current_process->next;

	//terminaison
	while(current_process->state == TERMINATED){
		pcb_s* tmp_process = current_process->next;

		current_process->previous->next = current_process->next;
		current_process->next->previous = current_process->previous;

		phyAlloc_free((void*)current_process->ctx->sp, current_process->stack_size);
		phyAlloc_free(current_process->ctx, sizeof(ctx_s));
		phyAlloc_free(current_process, sizeof(pcb_s));
	
		current_process = tmp_process;
	}
}

void start_sched(){
	current_process=first;
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



