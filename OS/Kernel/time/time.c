#include<time.h>
#include<video.h>
#include<list.h>
#include<scheduler.h>
#include<ipc.h>

static unsigned long long ticks = 0;


List * awakeTimes;


void initialize_timer(){
	awakeTimes=newList();
}

void * timer_handler(void * rsp) {
	ticks++;
	Node * minTicks=awakeTimes->start;
	while(minTicks != NULL && minTicks->hash <= ticks){
		awakeAll(minTicks->elem);		
		closeMotive(minTicks->elem);
		remove(awakeTimes,minTicks->hash);
		m_free(minTicks->elem);
		minTicks = awakeTimes->start;	
	}
	return schedule(rsp);
}

void sleep(unsigned int interval){
	PCB * proc = getCurrentProc();
	unsigned long awakeTime = interval*18+ticks;
	unsigned long * time = get(awakeTimes,awakeTime);
	if(time == NULL){
		time = (unsigned long * ) m_alloc(sizeof(unsigned long));
		if(time == NULL)
			return;
		*time = awakeTime;
		createMotive(time);
		add(awakeTimes,time,*time);
	}
	blockMotive(time,proc->pid);
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

