#ifndef SCHED_H
#define SCHED_H

#define STACK_SIZE 512
#define NULL 0

typedef void (*func_t) (void*);

typedef enum {NEW, READY, RUNNING, WAITING, TERMINATED} State;

typedef enum {LOW, NORMAL, HIGH, HIGHEST} Priority;

#define PRIORITY_NUM HIGHEST+1

#define INTERRUPT_TIME_CONST 1

#define INTERRUPT_TIME(priority) ((PRIORITY_NUM-priority+1)*INTERRUPT_TIME_CONST)

typedef struct ctx_s ctx_s;
struct ctx_s {
	unsigned int sp;
	unsigned int lr;
};

typedef struct pcb_s pcb_s;
struct pcb_s {
	State state;
	unsigned int sleepingTime;
	func_t function;
	void* functionArgs;
	ctx_s* ctx;
	unsigned int stack_size;
	pcb_s* next;
	pcb_s* previous;
	Priority priority;
};

typedef pcb_s* (*sched_func) ();

pcb_s* current_process;

pcb_s* IDLE;

typedef struct queue queue;
struct queue {
	pcb_s* first;
	pcb_s* last;
};

queue* queue_round_robin;

queue* queue_fixed_priority[PRIORITY_NUM];

void init_ctx(ctx_s*, unsigned int);

void init_sched();

void init_pcb(pcb_s*, func_t, void*, unsigned int, Priority);

void create_process(func_t, void*, unsigned int, Priority);

void add_pcb(queue*, pcb_s*);

void start_current_process();

void elect();

pcb_s* scheduler();

pcb_s* sched_round_robin();

pcb_s* sched_fixed_priority();

void __attribute__ ((naked)) ctx_switch_from_wait();

void start_sched();

void __attribute__ ((naked)) ctx_switch_from_irq();

void __attribute__ ((naked)) ctx_switch();

sched_func scheduler_function;

#endif
