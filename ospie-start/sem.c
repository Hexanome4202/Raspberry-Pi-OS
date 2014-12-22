#include "sem.h"

void sem_init(sem_s* sem, unsigned int val) {
	sem->counter = val;
}

void sem_up(sem_s* sem) {
	++(sem->counter);
	// Elect one process in the queue
}

void sem_down(sem_s* sem) {
	--(sem->counter);
	// Add one process to the queue
}
