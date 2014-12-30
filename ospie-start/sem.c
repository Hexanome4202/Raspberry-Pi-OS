#include "sem.h"
#include "hw.h"
#include "defines.h"

// TODO: change set_tick... to set normal waiting time or something smarter

void sem_init(sem_s* sem, unsigned int val) {
	sem->counter = val;
}

void sem_up(sem_s* sem) {
	DISABLE_IRQ();

	++(sem->counter);
	elect_blocked_process();

	set_tick_and_enable_timer();
	ENABLE_IRQ();
}

void sem_down(sem_s* sem) {
	DISABLE_IRQ();

	if(sem->counter > 0) {
		--(sem->counter);
	} else {
		// Add one process to the queue
	}
	set_tick_and_enable_timer();
	ENABLE_IRQ();
}
