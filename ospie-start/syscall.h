#ifndef SYSCALL_H
#define SYSCALL_H

unsigned int nbQuantums;

void sys_reboot();

void sys_wait(unsigned int nbQuantums);

void SWIHandler();

void doSysCallReboot();


#endif
