#ifndef _TIME_H_
#define _TIME_H_

#include <scheduler.h>

#define TIME_ELAPSED 55
#define MS_TO_S 1000

#define TO_SECONDS TIME_ELAPSED*1.0f/(MS_TO_S*1.0f)

#define WAITING 1
#define EMPTY 0


typedef struct WaiterHeader{
	struct Waiter * start;
	unsigned int size;
}WaiterHeader;

typedef struct Waiter{
	unsigned long long start; // = ticks
	unsigned long time; // = seconds
	PCB * proc;
	unsigned char status; // =0 killed ,1 waiting
	struct Waiter * next;
}Waiter;


void * timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void sleep(unsigned int time);
void initialize_timer();
#endif
