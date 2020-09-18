#include<registersArgs.h>

void cpyRegs(registerArgs * dest, registerArgs * src){
	for(int i = 0 ; i < REGISTERS ; i++){
		((uint64_t *) dest)[i] = ((uint64_t *) src)[i];
	}
}
