#include <time.h>

#include<video.h>

static unsigned long long ticks = 0;


WaiterHeader waiterHeader;

Waiter waiters[PROC_Q];

void initialize_timer(){
	waiterHeader.start=0;
	waiterHeader.size=0;
	for(int i = 0; i< PROC_Q; i++) waiters[i].status = EMPTY;
}

void * timer_handler(void * rsp) {
	ticks++;

	Waiter * prev = waiterHeader.start;
	Waiter * next = waiterHeader.start;
	while(next != 0){
		if( (ticks-next->start) * TO_SECONDS>= next->time){
			next->proc->state=READY;
			next->status = EMPTY;
			prev->next = next->next;
			if(prev==next){
				waiterHeader.start=next->next;
				prev=waiterHeader.start;
			}
			else prev = next;
		
		}
		else prev = next;

		next=next->next;
	}

	//fijarse si ya paso el tiempo

	return schedule(rsp);

}

void sleep(unsigned int time){
	PCB * proc = getCurrentProc();
	int i = 0;
	while(i<PROC_Q && waiters[i].status==WAITING)i++;
	if (i==PROC_Q) return; //TODO tiro error
	
	
	waiters[i].next=waiterHeader.start;
	waiterHeader.start = waiters+i;
	
	waiters[i].time=time;
	waiters[i].status=WAITING;
	waiters[i].proc=proc;
	waiters[i].start=ticks;
	
	proc->state=BLOCKED;
	yield();

}

void dec_tick(){
	ticks--;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

