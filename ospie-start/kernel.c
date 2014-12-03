#include "sched.h"
void funcA()
{
	int cptA = 0;
	while ( 1 ) {
		cptA ++;
	}
}
void funcB()
{
	int cptB = 1;
	while ( 1 ) {
		cptB += 2 ;
	}
}

//------------------------------------------------------------------------
int kmain ( void )
{
	init_hw();
	init_sched();
	create_process(funcB, NULL, STACK_SIZE, NORMAL);
	create_process(funcA, NULL, STACK_SIZE, LOW);
	start_sched();

	while (1)
		;

	/* Pas atteignable vues nos 2 fonctions */
	return 0;
}
