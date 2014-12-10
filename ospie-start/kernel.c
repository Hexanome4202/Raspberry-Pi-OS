#include "sched.h"
#include "vmem.h"
#include "translate.c"
#include "hw.h"
#include "syscall.h"
void funcA()
{
	int cptA = 0;
	sys_wait(3);
	while ( 1 ) {
		cptA ++;
		//ctx_switch();
	}
}
void funcB()
{
	int cptB = 1;
	//sys_wait(3);
	while ( 1 ) {
		cptB += 2 ;
		//ctx_switch();
	}
}

//------------------------------------------------------------------------
int kmain ( void )
{
	init_hw();
	init_sched();
	//create_process(funcB, NULL, STACK_SIZE, NORMAL);
	//create_process(funcA, NULL, STACK_SIZE, NORMAL);
	//create_process(init_kern_translation_table, NULL, STACK_SIZE, NORMAL);
	

	
	init_kern_translation_table();
	configure_mmu_C();
	unsigned int pa = translate(0x10022);
	start_mmu_C();

	
	
	create_process(funcB, NULL, STACK_SIZE,NORMAL);
	
	create_process(funcA, NULL, STACK_SIZE,NORMAL);
	start_sched();
	//ctx_switch();
	
	//sys_wait(3);

	//set_tick_and_enable_timer();
	//__asm volatile ("cpsie i");
	while (1)
		;

	/* Pas atteignable vues nos 2 fonctions */
	return 0;
}
