#include "vmem.h"

void
handle_data()
{
    unsigned int aborted_instruction_address;
    unsigned int data_fault, instr_fault;
    unsigned int status;
    unsigned int S_bit;
    unsigned int pa;

    __asm volatile("mov %[vloc], r0" : [vloc] "=r" (aborted_instruction_address));       /* lr - 8*/
    __asm volatile("mrc p15, 0, %[instr], c5, c0, 1" : [instr] "=r" (instr_fault));   /* instruction */
    __asm volatile("mrc p15, 0, %[data], c5, c0, 0" : [data] "=r" (data_fault));      /* data */

    status = data_fault & 0xF;
    S_bit = (data_fault >> 10) & 0x1;

    if (S_bit == 0)
    {
	switch(status)
	{
	case 0: /* No function */
	    break;
	case 1: /* Alignment fault */
	    break;
	case 2: /* Instruction debug event fault */
	    break;
	case 3: /* Access Bit fault on Section */
	    break;
	case 4: /* Instruction cache maintenance operation fault */
	    break;
	case 5: /* Translation Section fault */
	    break;
	case 6: /* Access Bit fault on Page */ 
	    break;
	case 7: /* Translation Page fault */
	    pa = translate(aborted_instruction_address);
	    break;
	case 8: /* Precise external abort */
	    break;
	case 9: /* Domain Section fault */
	    break;
	case 10: /* No function */
	    break;
	case 11: /* Domain Page fault */
	    break;
	case 12: /* External abort on translation, first level */
	    break;
	case 13: /* Permission Section fault */
	    break;
	case 14: /* External abort on translation, second level */
	    break;
	case 15: /* Permission Page fault */
	    break;
	default: /* Unreachable */
	    break;
	}
    }
    else
    {
	if (status == 6)
	{
	    /* Imprecise external abort */
	}
	else
	{
	    /* No function */
	}
    }
}
