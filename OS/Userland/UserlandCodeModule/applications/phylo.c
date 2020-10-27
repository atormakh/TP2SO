#include<stdlib.h>
#include<syscalls.h>
 
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+(num-1))%num
#define RIGHT (ph_num+1)%num
#define THINKING_TIME 1
#define EATING_TIME 1
#define MAX_PHILOSOPHERS 100
#define START_PHILOSOPHERS 5
 
void * mutex;
void * S[MAX_PHILOSOPHERS];
 
void *philospher(int argc, char * args[]);
void take_fork(int);
void put_fork(int);
void test(int);
void printPhilosophers();
void addPhilosopher();
void removePhilosopher();
 
int state[MAX_PHILOSOPHERS];
int phil_num[MAX_PHILOSOPHERS];
 
char buffer[64];
unsigned long long pids[MAX_PHILOSOPHERS];
int num;

char * getId(char * prefix,int id){
    int n=0;
    n+=strcpy(buffer,prefix);
    intToString(id,buffer+n);
    return buffer;
}

int phylo(){
    int i;

    sys_openSem("mutex",1);
    
    for(i=0;i<START_PHILOSOPHERS;i++){
        addPhilosopher();
    }

    
    char c;
    while(1){
        sys_readPipe(0,&c,1);
        if(c=='a'){
            addPhilosopher();
        }else if(c=='r'){
            removePhilosopher();
        }
        sys_yield();
    }

    for(i=0;i<num;i++)
        sys_wait(pids[i]);
}
 
void *philospher(int argc, char * args[]){
    
    int * num = stringToPtr(args[0]);
    while(1){
        int *i = num;

        sys_sleep(THINKING_TIME);
        
        take_fork(*i);
        
        sys_yield();
        
        put_fork(*i);
    }
}
 
void take_fork(int ph_num){
    sys_semWait("mutex");
    state[ph_num] = HUNGRY;
   
    test(ph_num);
    sys_semPost("mutex");
    sys_semWait(getId("sem_",ph_num));
    //sys_sleep(1);
}
 
void test(int ph_num){
    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
        state[ph_num] = EATING;
        sys_semPost("mutex");
        sys_sleep(EATING_TIME);
        sys_semWait("mutex");
        printPhilosophers();
        sys_semPost(getId("sem_",ph_num));
    }
}
 
void put_fork(int ph_num){
    sys_semWait("mutex");
    state[ph_num] = THINKING;
    printPhilosophers();
    test(LEFT);
    test(RIGHT);
    sys_semPost("mutex");
}



void printPhilosophers(){
    char bufferStart[100];
    char * bufferCurrent = bufferStart;
    for(int i=0;i<num;i++){
        switch(state[i]){
            case EATING:
                bufferCurrent+=strcpy(bufferCurrent," E ");
                break;
            case THINKING:
                bufferCurrent+=strcpy(bufferCurrent," . ");
                break;
            case HUNGRY:
                bufferCurrent+=strcpy(bufferCurrent," X ");
                break;
        }
    }
    printf("%s\n",bufferStart);
}

void addPhilosopher(){
    printf("%s \n", "adding");
    printPhilosophers();
    sys_semWait("mutex");
    char param[16]={0};
    char * args[]={param};
    phil_num[num]=num;
    sys_openSem(getId("sem_",num),0);
    intToString((unsigned long long)&phil_num[num],args[0]);
    pids[num]=sys_createProcess(philospher,"philospher",1,args);
    sys_unblock(pids[num]);
    num++;
    sys_semPost("mutex");
}

void removePhilosopher(){
    if(num<=0) return;
    if(state[num-1]==EATING)put_fork(num-1);
    sys_semWait("mutex");
    printf("%s \n", "removing");
    printPhilosophers();
    num--;
    sys_closeSem(getId("sem_",num));
    sys_kill(pids[num]);
    sys_semPost("mutex");
}