/*
 * syscall.c
 * 
 * 
 */
#include "syscall.h"
#include "sched.h"
#include "hw.h"

const int PM_RSTC = 0x2010001c;
const int PM_WDOG = 0x20100024;
const int PM_PASSWORD = 0x5a000000;
const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

void sys_reboot(){
	DISABLE_IRQ();

	__asm("mov r0, %0" : : "r"(SYS_REBOOT) : "r0");
	__asm("SWI 0" : : : "lr");
}
	
	
void __attribute__ ((naked)) SWIHandler(){
	unsigned int signal;
	__asm("mov %0, r0" : "=r"(signal));
	
	__asm("cps #0x13");
	
	if(signal == SYS_REBOOT) {
		doSysCallReboot();
	} else if(signal == SYS_WAIT) {
		__asm("mov %0, r1" : "=r"(nbQuantums));
		current_process->state = WAITING;
		current_process->sleepingTime = nbQuantums;
	
		__asm("b ctx_switch_from_wait");
	}
}
	
void __attribute__ ((naked)) sys_wait(unsigned int nbQuantums)
{
	DISABLE_IRQ();

	//1. sauvegarde le contexte du processus en cours d’exécution
	__asm("srsdb sp!, #0x13");
	__asm("push {r0-r12, lr}");
	__asm("mov %0, sp" : "=r"(current_process->ctx->sp));

	// Paramètres et appel de SWI handler
	__asm("mov r0, %0" : : "r"(SYS_WAIT) : "r0");
	__asm("mov r1, %0" : : "r"(nbQuantums) : "r1");
	__asm("SWI 0" : : : "lr");
}
	
void doSysCallReboot() {
	
	PUT32(PM_WDOG, PM_PASSWORD | 1);
	PUT32(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
	while(1);
	
}
