#include <time.h>
#include <stdint.h>
#include <keyboard.h>

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq, uint64_t * registers) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21(registers);
	}
	return;
}

void int_20() {
	timer_handler((void *)registers);
}
void int_21(uint64_t * registers){
	keyboard_handler((registerArgs * )registers);
}
