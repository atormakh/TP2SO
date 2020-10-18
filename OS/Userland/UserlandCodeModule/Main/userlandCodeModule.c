#include <screenDrawer.h>
#include <shell.h>
#include<syscalls.h>


int main() {
	unsigned long long pid1=0;
	unsigned long long pid2=0;
	pid1=sys_createProcess(screenDrawer,"X11", 0, 0);
	pid2=sys_createProcess(shell,"sh", 0, 0);
	sys_pipe(pid2,1,pid1,0);
	sys_unblock(pid1);
	sys_unblock(pid2);
	while(1)
		sys_yield();
	
}