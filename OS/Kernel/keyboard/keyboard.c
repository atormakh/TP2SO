#include<keyboard.h>
#include <video.h>
#include "asccodes.h"
#include<scheduler.h>
#include <ipc.h>
#include<lib.h>

#define BUFFER_SIZE 32
 
static char buffer[BUFFER_SIZE];
static int bufferIndex=0;
static int base=0;
char shiftLStatus=0;
char shiftRStatus=0;
char blockMayus=0;
char (* layout)[2]=asccode;

unsigned char init=0;

void keyboard_handler(){
    unsigned char key = getKey();
	if(key==SHIFT_L) shiftLStatus=1;
	else if(key==SHIFT_R) shiftRStatus=1;
	else if (key==SHIFT_R+RELEASED)shiftRStatus=0;
	else if(key==SHIFT_L+RELEASED) shiftLStatus=0;
	else if(key==BLOCK_MAYUS) blockMayus=1-blockMayus;
	else if(key == 59){ //F1
		char ps_buffer[1024];
		ps(ps_buffer);
		kernelWrite(ps_buffer,strlen(ps_buffer));
		//buffer[(bufferIndex++)%BUFFER_SIZE]=208;
	}else if(key == 60){ //F2
		layout=asccode;
	}else if(key == 61){ //F3
		layout=spanish_asccode;
	}else if(key<58)buffer[(bufferIndex++)%BUFFER_SIZE]= layout[key][( shiftRStatus | shiftLStatus | blockMayus) - ((shiftLStatus | shiftRStatus) & blockMayus)];
    
	if(init == 0){
		init = 1;
		createMotive(buffer);
	}
	awakeAll(buffer);
}


unsigned long long readKeyboard(char * buf, int count){
	//nos fijamos si hay algo escrito
	while(bufferIndex-base == 0){	//no hay nada escrito, asi que procedemos a bloquearlo
		//nos fijamos si ya esta creado el motivo del keyboard (que esta linkeado con la direccion del buffer)
		if(init == 0){
			init = 1;
			createMotive(buffer);
		}
		blockMotive(buffer,getCurrentProc()->pid);
		yield();
	}
	int index;
	for(index = 0; index<(bufferIndex-base) && index<count; index++){
		buf[index] = buffer[(base++)%BUFFER_SIZE];
	}
	return index;
}