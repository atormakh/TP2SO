#include <time.h>
#include <scheduler.h>

static unsigned long ticks = 0;

void timer_handler(void * rsp) {
	ticks++;
	rsp = schedule(rsp);
	return rsp;

}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

