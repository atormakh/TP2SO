#include<time.h>
#include<video.h>
#include<list.h>
#include<scheduler.h>

static unsigned long long ticks = 0;


List * awakeTimes;

int cmp(void * el1, void * el2){
	return *(unsigned long *)el1-*(unsigned long *)el2;
}

void initialize_timer(){
	awakeTimes=newList(cmp);
}

void * timer_handler(void * rsp) {
	ticks++;
	unsigned long * minTicks;
	if(awakeTimes->start != NULL && *(minTicks=awakeTimes->start->elem)==ticks){
		awakeAll(minTicks);		
		closeMotive(minTicks);
		remove(awakeTimes,minTicks);
		m_free(minTicks);
		
	}
	return schedule(rsp);

}

void sleep(unsigned int interval){
	PCB * proc = getCurrentProc();
	unsigned long awakeTime = interval*18+ticks;
	unsigned long * time = get(awakeTimes,&awakeTime);
	if(time == NULL){
		time = (unsigned long * ) m_alloc(sizeof(unsigned long));
		*time = awakeTime;
		createMotive(time);
		add(awakeTimes,time);
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

