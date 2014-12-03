//init table page
//init table frames
//activer MMU
//primitives alloc et libération des pages
#include "vmem.h"
#include <stdio.h>
#include <stdint.h>

void test(){
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

	printf("%04X\n", device_flags);
}