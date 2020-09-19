#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <memoryManager.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const userlandCodeModuleAddress = (void*)0x400000;
static void * const userlandDataModuleAddress = (void*)0x500000;
static void * const baseAddress = (void*)0x600000;

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

	clearBSS(&bss, &endOfKernel - &bss);
	
	return getStackBase();
}

int main()
{	
	load_idt();
	
	uint32_t memSize = *(uint32_t *)0x5020;

	initialize_mem_man(baseAddress ,PAGE_SIZE, (memSize-baseAddress)/PAGE_SIZE); // el ultimo parametro se puede eliminar, se calculan en funcion de parametros anteriores
	initialize_scheduler();
	createProcess(userlandCodeModuleAddress);
	schedule();
	while(1);
	return 0;
}