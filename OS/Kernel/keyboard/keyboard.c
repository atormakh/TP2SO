#include<keyboard.h>
#include <video.h>
#include <registersArgs.h>
#include "asccodes.h"

#define BUFFER_SIZE 32
 
static char buffer[BUFFER_SIZE];
static registerArgs registers;
static int i=0;
static int saveReg=0;
static int base=0;
char shiftLStatus=0;
char shiftRStatus=0;
char blockMayus=0;
char (* layout)[2]=asccode;

void keyboard_handler(registerArgs * regs){
    unsigned char key = getKey();
	if(key==SHIFT_L) shiftLStatus=1;
	else if(key==SHIFT_R) shiftRStatus=1;
	else if (key==SHIFT_R+RELEASED)shiftRStatus=0;
	else if(key==SHIFT_L+RELEASED) shiftLStatus=0;
	else if(key==BLOCK_MAYUS) blockMayus=1-blockMayus;
	else if(key==ALT_L || key == CTRL_L){
		saveReg=1;
		cpyRegs(&registers,regs); // con ctrl izq
	}else if(key == 59){ //F1
		buffer[(i++)%BUFFER_SIZE]=208;
	}else if(key == 60){ //F2
		layout=asccode;
	}else if(key == 61){ //F3
		layout=spanish_asccode;
	}else if(key==224){
		key=getKey();
		switch (key){
			case 80: // abajo
				key=201;
				buffer[(i++)%BUFFER_SIZE]=shiftLStatus? key+4 : key;
				break;
			case 75: // izq
				key=202;
				buffer[(i++)%BUFFER_SIZE]=shiftLStatus? key+4 : key;
				break;
			case 72: // arriba
				key=200;
				buffer[(i++)%BUFFER_SIZE]=shiftLStatus? key+4 : key;
				break;
			case 77: //derecha
				key=203;
				buffer[(i++)%BUFFER_SIZE]=shiftLStatus? key+4 : key;
				break;
		}

	}else if(key<58)buffer[(i++)%BUFFER_SIZE]= layout[key][( shiftRStatus | shiftLStatus | blockMayus) - ((shiftLStatus | shiftRStatus) & blockMayus)];
    
}

void retrieveRegs(registerArgs * args, int * flag){
	if(!saveReg){
		*flag = 0;
		return;
	}
	*flag=1;
	cpyRegs(args,&registers);
}

void readKeyboard(char * buf, int count, int * amount){
	int index;
	for(index = 0; index<(i-base) && index<count; index++){
		buf[index] = buffer[(base++)%BUFFER_SIZE];
	}

	*amount = index;
}