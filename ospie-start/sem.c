#include "sem.h"
#include "sched.h"
#include "hw.h"

void sem_init(sem_s* sem, unsigned int val) {
	sem->counter = val;
}

void sem_up(sem_s* sem) {
	DISABLE_IRQ();

	++(sem->counter);
	// Elect one process in the queue

	set_tick_and_enable_timer();
	ENABLE_IRQ();
}

void sem_down(sem_s* sem) {
	DISABLE_IRQ();

	--(sem->counter);
	// Add one process to the queue

	set_tick_and_enable_timer();
	ENABLE_IRQ();
}
