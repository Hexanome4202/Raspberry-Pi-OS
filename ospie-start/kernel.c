#include "sched.h"
#include "vmem.h"
#include "fb.h"
#include "translate.h"
#include "hw.h"
#include "syscall.h"
#include "gui.h"
#include "sem.h"

sem_s sem1, sem2;
int inccc = 0;

void funcA()
{
	int cptA = 0;
	sys_wait(3);
	while ( 1 ) {
		cptA ++;
	}
}
void funcB()
{
	int cptB = 1;
	while ( 1 ) {
		cptB += 2 ;
	}
}

void funcRed()
{
	while(1){
		drawRed();
	}
}

void funcBlue()
{
	while(1){
		drawBlue();
	}
}

void led() {
	while(1){
		led_on();
		sys_wait(4);
		led_off();
		sys_wait(4);
	}
}

void ledON() {
	while(1){
		led_on();
		sys_wait(5);
	}
}

void ledOFF() {
	while(1){
		led_off();
		sys_wait(5);
	}
}

void prod() {
	while(1) {
		sem_down(&sem1);
		++inccc;
		sem_up(&sem2);		
		sys_wait(1);
	}
}	

void cons() {
	while(1) {
		sem_down(&sem2);
		--inccc;		
		sem_up(&sem1);
		sys_wait(1);
	}
}

void paint() {
	while(1) {
		guiPainter();
	}
}

void movingR(){
	uint32 speedX =10, speedY=10;
	uint32 width = getWidth();
	uint32 height = getHeight();
	uint32 posX = 0, posY = 0;
	uint32 sizeSquare = 50;
	uint32 minX, maxX, minY, maxY;
	while(1) {
		minX = posX;
		minY = posY;
		
		addBlackSquare(posX,posY,sizeSquare,sizeSquare);
		addBlackSquare(posX,sizeSquare+posY,sizeSquare,sizeSquare);
		addBlackSquare(posX,(sizeSquare*2)+posY,sizeSquare,sizeSquare);
		addBlackSquare(posX,(sizeSquare*3)+posY,sizeSquare,sizeSquare);
		addBlackSquare(posX,(sizeSquare*4)+posY,sizeSquare,sizeSquare);
	
		addBlackSquare(sizeSquare+posX,posY,sizeSquare,sizeSquare);
		addBlackSquare(sizeSquare*2+posX,sizeSquare+posY,sizeSquare,sizeSquare);
		addBlackSquare(sizeSquare+posX,sizeSquare*2+posY,sizeSquare,sizeSquare);
	
		addBlackSquare(sizeSquare*2+posX,sizeSquare*3+posY,sizeSquare,sizeSquare);
		addBlackSquare(sizeSquare*3+posX,sizeSquare*4+posY,sizeSquare,sizeSquare);
		
		if(posX+(sizeSquare*4) >= width){
			speedX = -1*speedX;
		}
		else if(posX <= 0 && speedX < 0){
			speedX = -1*speedX;	
		}
		if(posY+(sizeSquare*5) >= height){
			speedY = -1*speedY;
		}
		else if(posY <= 0 && speedY < 0){
			speedY = -1*speedY;	
		}
		posX += speedX;
		posY += speedY;

		maxX = posX+sizeSquare*5;
		maxY = posY+sizeSquare*5;
		if(maxX > width){
			maxX= width;
		}
		if(maxY > height){
			maxY = height;
		}

		addWhiteSquare(posX,posY,sizeSquare,sizeSquare);
		addWhiteSquare(posX,sizeSquare+posY,sizeSquare,sizeSquare);
		addWhiteSquare(posX,(sizeSquare*2)+posY,sizeSquare,sizeSquare);
		addWhiteSquare(posX,(sizeSquare*3)+posY,sizeSquare,sizeSquare);
		addWhiteSquare(posX,(sizeSquare*4)+posY,sizeSquare,sizeSquare);
	
		addWhiteSquare(sizeSquare+posX,posY,sizeSquare,sizeSquare);
		addWhiteSquare(sizeSquare*2+posX,sizeSquare+posY,sizeSquare,sizeSquare);
		addWhiteSquare(sizeSquare+posX,sizeSquare*2+posY,sizeSquare,sizeSquare);
	
		addWhiteSquare(sizeSquare*2+posX,sizeSquare*3+posY,sizeSquare,sizeSquare);
		addWhiteSquare(sizeSquare*3+posX,sizeSquare*4+posY,sizeSquare,sizeSquare);
		guiPainter(minX,maxX,minY,maxY);
	}
}

void movingText() {
	// FIXME
	//char text[3];
	//text[0] = 'A';
	//text[1] = 'A';
	//text[2] = 'A';
	char* text = "REMI";
	int i = 0;
	while(1) {
		display_text(text, 3, 20 + i, 20 + i);
		++i;
		sys_wait(1);
	}
}

void forms() {
	int i;
	for(i = 0; i < getWidth(); ++i) {
		drawCircle(100 + i, 100 + i, 40, 120, 210, 18);
		drawLine(10 + i, 100 + i, 100 + i, 10 + i, 250, 0, 0);
		//sys_wait(1);
	}
}

//------------------------------------------------------------------------
int kmain ( void )
{

	init_hw();
	init_sched();

	//create_process(funcB, NULL, STACK_SIZE, LOW);
	//create_process(funcA, NULL, STACK_SIZE, NORMAL);
	//create_process(init_kern_translation_table, NULL, STACK_SIZE, NORMAL);

	FramebufferInitialize();
	guiInitialize();
	//draw();

	//display_text("lol", 3, 20, 20);

	sem_init(&sem1, 1);
	sem_init(&sem2, 0);

	//create_process(ledON,NULL,STACK_SIZE, NORMAL);
	//create_process(funcRed, NULL, STACK_SIZE, NORMAL);
	//create_process(ledOFF,NULL,STACK_SIZE, NORMAL);	
	//create_process(funcBlue, NULL, STACK_SIZE, NORMAL);
	//create_process(led_off,NULL,STACK_SIZE, LOW);
	//create_process(prod, NULL, STACK_SIZE, NORMAL);
	//create_process(cons, NULL, STACK_SIZE, NORMAL);
	//create_process(paint, NULL, STACK_SIZE, NORMAL);
	//create_process(movingR, NULL, STACK_SIZE, NORMAL);
	create_process(movingText, NULL, STACK_SIZE, NORMAL);
	//create_process(forms, NULL, STACK_SIZE, NORMAL);

	start_sched();
	
	//audio_test();

	while(1) ;

	/* Pas atteignable vues nos 2 fonctions */
	return 0;
}



	/** TITOUAN AND JUSTINE WORK
	init_kern_translation_table();
	configure_mmu_C();
	unsigned int pa = translate(0x10022);
	start_mmu_C();
	
	uint8_t* adresse = vMem_Alloc(5);
	unsigned int pa1 = translate(adresse);
	
	start_sched();

**/
