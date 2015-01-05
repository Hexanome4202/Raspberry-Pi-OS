#include "sched.h"
#include "vmem.h"
#include "fb.h"
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
	while(1){
		drawRed();
	}
}

void funcBlue()
{
	while(1){
		drawBlue();
	}
}

void led() {
	while(1){
		led_on();
		sys_wait(4);
		led_off();
		sys_wait(4);
	}
}

void ledON() {
	while(1){
		led_on();
		sys_wait(5);
	}
}

void ledOFF() {
	while(1){
		led_off();
		sys_wait(5);
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

	/*
	create_process(ledON,NULL,STACK_SIZE, NORMAL);
	create_process(funcRed, NULL, STACK_SIZE, NORMAL);
	create_process(ledOFF,NULL,STACK_SIZE, NORMAL);	
	create_process(funcBlue, NULL, STACK_SIZE, NORMAL);
	//create_process(led_off,NULL,STACK_SIZE, LOW);

	
	/** TITOUAN AND JUSTINE WORK */

	init_kern_translation_table();
	configure_mmu_C();
	unsigned int pa = translate(0x10022);
	start_mmu_C();
	
	uint32_t* adresse = vMem_Alloc(5);
	unsigned int pa1 = translate(adresse);

	uint32_t* adresse2 = vMem_Alloc(10);
	unsigned int pa2 = translate(adresse2);
	
	start_sched();

	while(1) ;

	/* Pas atteignable vues nos 2 fonctions */
	return 0;
}
