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
		| (1 << 2)
		| (1 << 3)
		| (0b11 << 4)
		| (0b001 << 6)
		| (0 << 9)
		| (0 << 10)
		| (0 << 11));

	//printf("%04X\n", device_flags);

	//printf("%032X\n", device_flags);
	uint32_t i=0;
	for(i=0; i < FIRST_LVL_TT_COUN; i++){
		
		uint32_t* entry_add = (uint32_t*) ((uint32_t)ADDR_TAB_LVL_1 + 4*i);
		uint32_t entry = (ADDR_TAB_LVL_1 + FIRST_LVL_TT_SIZE + i*SECON_LVL_TT_SIZE) << 10;

		*(entry_add) = entry;

		if(ADDR_TAB_LVL_1+4*i < 0x500000 || (ADDR_TAB_LVL_1+4*i > 0x20000000 && ADDR_TAB_LVL_1+4*i < 0x20FFFFFF)){
			*(entry_add) |= (0b01);
			i=i;
			//printf("%032X\n", *(entry_add));
		}
	}

/*
	for ()
		for()

			if (virtAdd >= 0x 20000000 && virtAdd < 0x 20FFFFFF)
			*(second_level_desc_add) = () | device_flags;
*/
}
