//init table page
//init table frames
//activer MMU
//primitives alloc et libération des pages
#include "vmem.h"
#include <stdio.h>
#include <stdint.h>


unsigned int init_kern_translation_table(){
	// 0	
	uint32_t device_flags = 
		  ((0 << 0)
		| (1 << 1)
		| (0 << 2)
		| (0 << 3)
		| (0b11 << 4)
		| (0b001 << 6)
		| (0 << 9)
		| (0 << 10)
		| (0 << 11));

	uint32_t device_flagsIO = 
		  ((0 << 0)
		| (0 << 1)
		| (1 << 2)
		| (0 << 3)
		| (0b00 << 4)
		| (0b000 << 6)
		| (0 << 9)
		| (0 << 10)
		| (0 << 11));	  

	//printf("%04X\n", device_flags);

	//printf("%032X\n", device_flags);
	uint32_t i=0;
	uint32_t* entry_add;
	for(i=0; i < FIRST_LVL_TT_COUN; i++){
		
		entry_add = (uint32_t*) ((uint32_t)ADDR_TAB_LVL_1 + 4*i);
		uint32_t entry = (ADDR_TAB_LVL_1 + FIRST_LVL_TT_SIZE + i*SECON_LVL_TT_SIZE) << 10;

		*(entry_add) = entry;

		if(ADDR_TAB_LVL_1+4*i < 0x500000 || (ADDR_TAB_LVL_1+4*i > 0x20000000 && ADDR_TAB_LVL_1+4*i < 0x20FFFFFF)){
			*(entry_add) |= (0b01);
			i=i;
			//printf("%032X\n", *(entry_add));
		}
	}



	for(i=0; i < FIRST_LVL_TT_COUN; i++){
			uint32_t j=0;
		for(j=0; j<SECON_LVL_TT_COUN; j++){

			entry_add=entry_add+4;
			uint32_t* phys_add =  (uint32_t*)((uint32_t)i*SECTION_SIZE+j*PAGE_SIZE);

			if( phys_add > 0x20000000 && phys_add< 0x20FFFFFF){
				
				uint32_t entry2 = (i*SECTION_SIZE+j*PAGE_SIZE)<<12 | device_flagsIO;
				*(phys_add)= entry2;

			}else if(i==0){
				
				uint32_t entry2 = (i*SECTION_SIZE+j*PAGE_SIZE)<<12 | device_flags;
				*(phys_add)= entry2;

			}else{
				*(phys_add)= 0;
			}
		}
	}

}

void start_mmu_C()
{
	register unsigned int control;
	__asm("mcr p15, 0, %[zero], c1, c0, 0" : : [zero] "r"(0)); //Disable cache
	__asm("mcr p15, 0, r0, c7, c7, 0"); //Invalidate cache (data and instructions) */
	__asm("mcr p15, 0, r0, c8, c7, 0"); //Invalidate TLB entries
	/* Enable ARMv6 MMU features (disable sub-page AP) */
	__asm volatile("mrc p15, 0 , %[control], c1, c0,0": [control] "=r"(control));
	control = (1<<23) | (1 << 15) | (1 << 4) | 1;
	/* Invalidate the translation lookaside buffer (TLB) */
	__asm volatile("mcr p15, 0, %[data], c8, c7, 0" : : [data] "r" (0));
	/* Write control register */
	__asm volatile("mcr p15, 0, %[control], c1, c0, 0" : : [control] "r" (control));
}

void configure_mmu_C()
{
	register unsigned int pt_addr = ADDR_TAB_LVL_1;
	//total++;
	/* Translation table 0 */
	__asm volatile("mcr p15, 0, %[addr], c2, c0, 0" : : [addr] "r" (pt_addr));
	/* Translation table 1 */
	__asm volatile("mcr p15, 0, %[addr], c2, c0, 1" : : [addr] "r" (pt_addr));
	/* Use translation table 0 for everything */
	__asm volatile("mcr p15, 0, %[n], c2, c0, 2" : : [n] "r" (0));
	/* Set Domain 0 ACL to "Manager", not enforcing memory permissions
	* Every mapped section/page is in domain 0
	*/
	__asm volatile("mcr p15, 0, %[r], c3, c0, 0" : : [r] "r" (0x3));
}
