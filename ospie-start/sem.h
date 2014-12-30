#ifndef SEM_H
#define SEM_H

#include "sched.h"

typedef struct sem_s sem_s;
struct sem_s {
	int counter;
	pcb_s* process;
	sem_s* next;
	sem_s* previous;
};

void sem_init(sem_s*, unsigned int);

void sem_up(sem_s*);

void sem_down(sem_s*);

#endif
