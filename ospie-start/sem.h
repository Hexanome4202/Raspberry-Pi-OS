#ifndef SEM_H
#define SEM_H

typedef struct sem_s sem_s;
struct sem_s {
	int counter;
	// Ajouter liste de processus bloqués
};

void sem_init(sem_s*, unsigned int);

void sem_up(sem_s*);

void sem_down(sem_s*);

#endif
