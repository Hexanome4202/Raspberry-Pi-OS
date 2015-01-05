//init table page
//init table frames
//activer MMU
//primitives alloc et libération des pages
#include "vmem.h"
#include "translate.c"

extern void SET32(unsigned int variable, int valeur);
extern int GET32(unsigned int variable);


unsigned int init_kern_translation_table(){
	// 0	
	uint32_t device_flags = 
		  ((0 << 0)
		| (1 << 1)
		| (0 << 2)
		| (0 << 3)
		| (0b00 << 4)
		| (0b000 << 6)
		| (0 << 9)
		| (0 << 10)
		| (0 << 11));

	uint32_t device_flagsIO = 
		  ((0 << 0)
		| (1 << 1)
		| (0 << 2)
		| (0 << 3)
		| (0b00 << 4)
		| (0b010 << 6)
		| (0 << 9)
		| (0 << 10)
		| (0 << 11));	  

	//printf("%04X\n", device_flags);

	//printf("%032X\n", device_flags);
	uint32_t i=0;
	uint32_t entry_add;
	for(i=0; i < FIRST_LVL_TT_COUN; i++){
		
		entry_add = ADDR_TAB_LVL_1 + 4*i;
		uint32_t entry = ADDR_TAB_LVL_1 + FIRST_LVL_TT_SIZE + i*SECON_LVL_TT_SIZE;
		SET32(entry_add,entry|(0b01));
		//*(entry_add) = entry|(0b01);
			//printf("%032X\n", *(entry_add));
	}

	for(i=0; i < FIRST_LVL_TT_COUN; i++){
		uint32_t j=0;
		for(j=0; j<SECON_LVL_TT_COUN; j++){

			entry_add +=4;
			uint32_t phys_add =  i*SECTION_SIZE+j*PAGE_SIZE;

			if( phys_add > 0x20000000 && phys_add< 0x20FFFFFF){
				
				uint32_t entry2 = (phys_add|device_flagsIO);
				SET32(entry_add,entry2);
				//*(entry_add)= entry2;

			} else if(phys_add >= ADDR_TAB_FRAME_OCC && phys_add<= 0x20000000){
				uint32_t entry2 = (phys_add| device_flags);
				SET32(entry_add,entry2);

			} else if(i<5) {
				
				uint32_t entry2 = (phys_add| device_flags);
				SET32(entry_add,entry2);
				//*(entry_add)= entry2;

			}else{
				SET32(entry_add,0);
				//*(entry_add)= 0;
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
	//__asm volatile("mrc p15, 0 , %[control], c1, c0,0": [control] "=r"(control));
	control = (1<<23) | (1 << 15) | (1 << 4) | 1;
	/* Invalidate the translation lookaside buffer (TLB) */
	__asm volatile("mcr p15, 0, %[data], c8, c7, 0" : : [data] "r" (0));
	/* Write control register */
	__asm volatile("mcr p15, 0, %[control], c1, c0, 0" : : [control] "r" (control));

	init_frame_tab();
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

void init_frame_tab(){
	
	uint32_t i;
	for(i=1; i< TAB_FRAME_SIZE; i++){
		uint32_t addr = ADDR_TAB_FRAME_OCC+i;
		translate(addr);
		if(i< 50000/4 || i>0x1FFDEFFF){
			SET32(addr, 1);
		}else{
			SET32(addr, 0);
		}
	}
}

uint32_t* vMem_Alloc(unsigned int nbPages){

	
	
	uint32_t secondLevelTtAdd = ADDR_TAB_LVL_1 + 4 * FIRST_LVL_TT_COUN;
	int nbFound = 0;
	uint32_t first = 0;


	//Parcours des adresses virtuelles
 	// 553648127 = 20FFFFFF
 	uint32_t i;
	for(i = 0 ; i < 553648127 && nbFound < nbPages ; i+= 4096){
		if(translate(i) == 0){
			if(nbFound == 0)
				first = i;

			nbFound++;
		}else{
			nbFound = 0;
		}
	}

	//Parcours de la table d'occupation
	//20FFFFFF/ 4096 =  135 168
	int j;
	for(j =0; j< nbPages; j++){
		uint32_t vir_add = first + 4096*j;

		//i index dans la table d'occupation + adresse de la table d'occupation + l'index de la première adresse utilisable
		for(i=ADDR_TAB_FRAME_OCC+50004/4; i< 135168+ADDR_TAB_FRAME_OCC; i++ ){
			if(GET32(i) == 0){
				uint32_t adrresse = i-ADDR_TAB_FRAME_OCC;
				uint32_t phys_add = adrresse*4096;

				//PASSER TABLE D'OCCUPATION A 1
				SET32(i ,1);

				addTranslation(vir_add, phys_add);
				break;
			}
		}
	}

	return (uint32_t*) first;

}

void
addTranslation(uint32_t va, uint32_t pa)
{	
	unsigned int table_base;
	 __asm("mrc p15, 0, %[tb], c2, c0, 0" : [tb] "=r"(table_base));
  
  	table_base = table_base & 0xFFFFC000;

	uint32_t flags = 
		  ((0 << 0)
		| (1 << 1)
		| (0 << 2)
		| (0 << 3)
		| (0b00 << 4)
		| (0b000 << 6)
		| (0 << 9)
		| (0 << 10)
		| (0 << 11));

	uint32_t entry2 = (pa|flags);


	uint32_t first_level_index = (va >> 20);
  	uint32_t second_level_index = ((va << 12) >> 24);
  	uint32_t page_index = (va & 0x00000FFF);

  	/* First level descriptor */
  	uint32_t* first_level_descriptor_address = (unsigned int*) (table_base | (first_level_index << 2));
  	uint32_t first_level_descriptor = *(first_level_descriptor_address);

  	/* Second level descriptor */
  	uint32_t second_level_table = first_level_descriptor & 0xFFFFFC00;
  	uint32_t* second_level_descriptor_address = (unsigned int*) (second_level_table | (second_level_index << 2));
  	uint32_t second_level_descriptor = *((unsigned int*) second_level_descriptor_address);    

  	SET32(second_level_descriptor_address,entry2);
}


void vMem_Free(uint32_t* ptr, unsigned int nbPages){

	uint8_t i;
	for(i=0; i< nbPages; i++){
		//GET(ptr+i*4) permet d'obtenir l'adresse physique
		uint32_t* phy_add = GET(ptr+i*4);

		SET(phys_add/4096+ADDR_TAB_FRAME_OCC, 0);
		SET(ptr+i, 0);

		//Ensuite faut aller dans la table des pages de niveau 2 et enlever le pointeur vers l'adresse physique
		//(En gros faut faire un SET(.., 0))
	}
}

