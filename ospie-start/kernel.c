#include "sched.h"
#include "vmem.h"
#include "fb.h"
#include "translate.h"
#include "hw.h"
#include "syscall.h"
#include "gui.h"
#include "sem.h"
#include "pwm.h"

sem_s sem1, sem2;
int inccc = 0;

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

void prod() {
	while(1) {
		sem_down(&sem1);
		++inccc;
		sem_up(&sem2);		
		sys_wait(1);
	}
}	

void cons() {
	while(1) {
		sem_down(&sem2);
		--inccc;		
		sem_up(&sem1);
		sys_wait(1);
	}
}

void paint() {
	while(1) {
		guiPainter();
		sys_wait(5);
	}
}

//------------------------------------------------------------------------
int kmain ( void )
{

	init_hw();
	//uint8 tab[800][600];
	init_sched();

	//create_process(funcB, NULL, STACK_SIZE, LOW);
	//create_process(funcA, NULL, STACK_SIZE, NORMAL);
	//create_process(init_kern_translation_table, NULL, STACK_SIZE, NORMAL);

	FramebufferInitialize();
	//guiInitialize();
	//draw();
	//guiPainter();

	sem_init(&sem1, 1);
	sem_init(&sem2, 0);

	//create_process(ledON,NULL,STACK_SIZE, NORMAL);
	//create_process(funcRed, NULL, STACK_SIZE, NORMAL);
	//create_process(ledOFF,NULL,STACK_SIZE, NORMAL);	
	//create_process(funcBlue, NULL, STACK_SIZE, NORMAL);
	//create_process(led_off,NULL,STACK_SIZE, LOW);
	//create_process(prod, NULL, STACK_SIZE, NORMAL);
	//create_process(cons, NULL, STACK_SIZE, NORMAL);
	//create_process(paint, NULL, STACK_SIZE, NORMAL);

	start_sched();
	
	audio_test();

	while(1) ;

	/* Pas atteignable vues nos 2 fonctions */
	return 0;
}
