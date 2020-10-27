#include <stdint.h>
#include <scheduler.h>
#include <lib.h>
#include <ipc.h>
#include<video.h>

char buffer[64];
char * regs[]={"r15","r14","r13","r12","r11","r10","r9 ","r8 ","rsi","rdi","rbp","rdx","rcx","rbx","rax"};
void exceptionDispatcher(int exception, uint64_t * registers) {
	
	kernelWrite("PROCESS ",8);
	int n=intToString(getCurrentProc()->pid,buffer);
	kernelWrite(buffer,n);
	kernelWrite("\n",1);
	kernelWrite("EXCEPTION ",10);
	n=intToString(exception,buffer);
	kernelWrite(buffer,n);
	kernelWrite("\n",1);
	kernelWrite("\n",1);
	 for(int i=0;i<15;i++){
	 	kernelWrite("\n",1);
		kernelWrite(regs[i],3);
		kernelWrite("   ",3);
		n=intToBase(registers[i],16,buffer);
		kernelWrite(buffer,n);
	}
	exit(0);
}
