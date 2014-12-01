#ifndef SCHED_H
#define SCHED_H

#define STACK_SIZE 512
#define NULL 0

typedef void (*func_t) (void*);
typedef enum {NEW, READY, RUNNING, WAITING, TERMINATED} State;

typedef struct ctx_s ctx_s;
struct ctx_s {
	unsigned int sp;
	unsigned int lr;
};

typedef struct pcb_s pcb_s;
struct pcb_s {
	State state;
	func_t function;
	void* functionArgs; 
	ctx_s* ctx;
	unsigned int stack_size;
	pcb_s* next;
	pcb_s* previous;
};

pcb_s* current_process;

pcb_s* first;
pcb_s* last;

void init_ctx(struct ctx_s*, unsigned int);

void init_pcb(pcb_s*, func_t, void*, unsigned int);

void create_process(func_t, void*, unsigned int);

void start_current_process();

void elect();

void start_sched();

void __attribute__ ((naked)) ctx_switch_from_irq();

void __attribute__ ((naked)) ctx_switch();

#endif

