#ifndef SYSCALL_H
#define SYSCALL_H

#define SYS_REBOOT 1
#define SYS_WAIT 2

unsigned int nbQuantums;

void sys_reboot();

void sys_wait(unsigned int nbQuantums);

void SWIHandler();

void doSysCallReboot();


#endif
