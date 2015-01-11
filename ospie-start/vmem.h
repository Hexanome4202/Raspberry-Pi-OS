#ifndef VMEM_H
#define VMEM_H

#include <stdio.h>
#include <stdint.h>


#define PAGE_SIZE 4096 //4Ko

//Nombre d'entrées contenues par une table de niveau 2
#define SECON_LVL_TT_COUN 256

#define SECON_LVL_TT_SIZE 1024

#define FIRST_LVL_TT_COUN 1024

#define FIRST_LVL_TT_SIZE 4096 //4Ko

#define SECTION_SIZE 1048576 //1MB

#define TOTAL_TT_SIZE 1052672

#define ADDR_TAB_LVL_1 0x48000

#define ADDR_TAB_FRAME_OCC 0x1FFDEFFF

#define TAB_FRAME_SIZE 135168

unsigned int init_kern_translation_table();

void start_mmu_C();

void configure_mmu_C();

void init_frame_tab();

uint8_t* vMem_Alloc(unsigned int nbPages);

void vMem_Free(uint8_t* ptr, unsigned int nbPages);

#endif