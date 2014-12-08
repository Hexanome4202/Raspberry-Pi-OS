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
	__asm("mov r0, %0" : : "r"(1) : "r0");
	__asm("SWI 0" : : : "lr");
	}
	
	
void __attribute__ ((naked)) SWIHandler(){
	unsigned int signal;
	__asm("mov %0, r0" : "=r"(signal));
	
	DISABLE_IRQ(); //Disable les interruptions
	
	__asm("cps #0x13");
	
	if(signal==1) {
		doSysCallReboot();
	} else if(signal==2) {
		
		current_process->state = WAITING;
		current_process->sleepingTime = nbQuantums;
	
		__asm("b ctx_switch_from_wait");
	}
		
	ENABLE_IRQ(); //Enable les interruptions
	
	
	}

void sys_wait(unsigned int nbQuantums)
{
	
	__asm("mov r0, %0" : : "r"(2) : "r0");
	__asm("mov r1, %0" : : "r"(nbQuantums) : "r1");
	__asm("SWI 0" : : : "lr");
	
	
}
	
void doSysCallReboot() {
	
	PUT32(PM_WDOG, PM_PASSWORD | 1);
	PUT32(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
	while(1);
	
}
