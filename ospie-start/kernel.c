#include "sched.h"
#include "vmem.h"
#include "fb.h"
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

void funcRed()
{
	//int i;
	//for(i=0; i<3; i++) {
	while(1){
		drawRed();
	}
}

void funcBlue()
{
	//int i;
	//for(i=0; i<5; i++) {
	while(1){
		drawBlue();
	}
}

//------------------------------------------------------------------------
int kmain ( void )
{
	init_hw();
	init_sched();

	//create_process(funcB, NULL, STACK_SIZE, LOW);
	//create_process(funcA, NULL, STACK_SIZE, NORMAL);
	//create_process(init_kern_translation_table, NULL, STACK_SIZE, NORMAL);

	FramebufferInitialize();
	draw();	

	//create_process(led_on,NULL,STACK_SIZE, HIGH);
	create_process(funcRed, NULL, STACK_SIZE, NORMAL);
	create_process(funcBlue, NULL, STACK_SIZE, NORMAL);

	create_process(led_off,NULL,STACK_SIZE, LOW);

	
	/** TITOUAN AND JUSTINE WORK
	init_kern_translation_table();
	configure_mmu_C();
	unsigned int pa = translate(0x10022);
	start_mmu_C();
	
	create_process(funcB, NULL, STACK_SIZE,NORMAL);
	create_process(funcA, NULL, STACK_SIZE,NORMAL);
	**/
	start_sched();

	while(1) ;

	/* Pas atteignable vues nos 2 fonctions */
	return 0;
}
