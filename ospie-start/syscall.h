#ifndef SYSCALL_H
#define SYSCALL_H

void sys_reboot();

void sys_wait(unsigned int nbQuantums);

void SWIHandler();

void doSysCallReboot();

void doSysCallWait(unsigned int nbQuantums);

#endif
