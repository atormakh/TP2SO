
#include <screenDrawer.h>
#include <shell.h>

int main() {
	unsigned long long pid1=0;
	unsigned long long pid2=0;
	sys_createProcess(screenDrawer, 0, 0,&pid1);
	sys_createProcess(shell, 0, 0,&pid2);
	sys_pipe(pid1,1,pid2,0);
	while(1)
		hlt();
	
}