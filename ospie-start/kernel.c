#include "sched.h"
void funcA()
{
	int cptA = 0;
	while ( 0 ) {
		cptA ++;
		//ctx_switch();
	}
}
void funcB()
{
	int cptB = 1;
	while ( 0 ) {
		cptB += 2 ;
		//ctx_switch();
	}
}

//------------------------------------------------------------------------
int kmain ( void )
{
	init_hw();
	init_sched();
	create_process(funcB, NULL, STACK_SIZE, NORMAL);
	create_process(funcA, NULL, STACK_SIZE, NORMAL);
	start_sched();
	//ctx_switch();

	while (1)
		;

	/* Pas atteignable vues nos 2 fonctions */
	return 0;
}
