#include <time.h>
#include <scheduler.h>

static unsigned long ticks = 0;

void * timer_handler(void * rsp) {
	ticks++;
	return schedule(rsp);

}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

