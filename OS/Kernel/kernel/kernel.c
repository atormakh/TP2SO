#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <memorymanager.h>
#include <scheduler.h>
#include <pipes.h>
#include <time.h>
#include <video.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const userlandCodeModuleAddress = (void*)0x400000;
static void * const userlandDataModuleAddress = (void*)0x500000;
static void * const baseAddress = (void*)0x800000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{	
	void * moduleAddresses[] = {
		userlandCodeModuleAddress,
		userlandDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, (unsigned long long)&endOfKernel - (unsigned long long)&bss);
	
	return getStackBase();
}




int main()
{	
	uint32_t memSize = *(uint32_t *)0x5020*1024*1024;
	initialize_mem_man(baseAddress ,memSize,PAGE_SIZE); // el ultimo parametro se puede eliminar, se calculan en funcion de parametros anteriores

	initialize_timer();
	initialize_scheduler();
	initPipes();
	createProcess(userlandCodeModuleAddress,"init",0,0);
	unblock(0);
	 
	load_idt();
	while(1);
	return 0;
}
