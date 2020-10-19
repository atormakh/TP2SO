#include<stdlib.h>
#include<syscalls.h>
 
#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+4)%N
#define RIGHT (ph_num+1)%N
 
void * mutex;
void * S[N];
 
void *philospher(int argc, char * args[]);
void take_fork(int);
void put_fork(int);
void test(int);
 
int state[N];
int phil_num[N]={0,1,2,3,4};
 
char buffer[64];

char * getId(int id){
    int n=0;
    n+=strcpy(buffer,"sem_");
    intToString(id,buffer+n);
    buffer[n]=0;
    return buffer;
}

int phylo(){
    int i;
    unsigned long long pids[N];
    char param[16];
    char * args[]={param};
    sys_openSem("mutex",1);
    for(i=0;i<N;i++)
        sys_openSem(getId(i),0);
    for(i=0;i<N;i++)
    {
        intToString(&phil_num[i],args[0]);
        pids[i]=sys_createProcess(philospher,"philospher",1,args);
        sys_unblock(pids[i]);
        printf("Philosopher %d is thinking \n",i+1);
    }
    for(i=0;i<N;i++)
        sys_wait(pids[i]);
}
 
void *philospher(int argc, char * args[]){
    
    int * num = stringToPtr(args[0]);
    while(1)
    {
        int *i = num;
        sys_sleep(1);
        take_fork(*i);
        sys_sleep(0);
        put_fork(*i);
    }
}
 
void take_fork(int ph_num){
    sys_semWait("mutex");
    state[ph_num] = HUNGRY;
    printf("Philosopher %d is Hungry \n",ph_num+1);
    test(ph_num);
    sys_semPost("mutex");
    sys_semWait(getId(ph_num));
    sys_sleep(1);
}
 
void test(int ph_num)
{
    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[ph_num] = EATING;
        sys_sleep(2);
        printf("Philosopher %d takes fork %d and %d \n",ph_num+1,LEFT+1,ph_num+1);
        printf("Philosopher %d is Eating \n",ph_num+1);
        sys_semPost(getId(ph_num));
    }
}
 
void put_fork(int ph_num)
{
    sys_semWait("mutex");
    state[ph_num] = THINKING;
    printf("Philosopher %d putting fork %d and %d down \n",ph_num+1,LEFT+1,ph_num+1);
    printf("Philosopher %d is thinking \n",ph_num+1);
    test(LEFT);
    test(RIGHT);
    sys_semPost("mutex");
}