#include "sem.h"
#include "hw.h"
#include "defines.h"

// TODO: change set_tick... to set normal waiting time or something smarter

void sem_init(sem_s* sem, unsigned int val) {
	sem->counter = val;
	sem->process = get_current_process();
	sem->next = NULL;
	sem->previous = NULL;
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

	if(sem->counter > 0) {
		--(sem->counter);
	} else {
		// Add one process to the queue
	}
	set_tick_and_enable_timer();
	ENABLE_IRQ();
}
