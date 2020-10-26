#include <stdint.h>
#include<syscalls.h>
#include<stdlib.h>

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"
#define TESTS_QTY "1000000"

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  *p = aux;
}

void inc(int argc, char * args[]){
  

  uint64_t sem = stringToInt(args[0]);
  int value = stringToInt(args[1]); 
  uint64_t N = stringToInt(args[2]);
  uint64_t i;

  if(sem)sys_openSem(SEM_ID,1);

  
  for (i = 0; i < N; i++){

    if (sem) sys_semWait(SEM_ID);
    slowInc(&global, value); 
    if (sem) sys_semPost(SEM_ID);
  }

  
  if (sem) sys_closeSem(SEM_ID);
  
  printf("Final value: %d\n", global);
  sys_exit(0);
}

void test_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n");

  char * args1[]={"1", "1",TESTS_QTY};
  char * args2[]={"1","-1", TESTS_QTY};
  unsigned long long pids[TOTAL_PAIR_PROCESSES*2];
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    pids[2*i]=sys_createProcess(inc,"inc", 3, args1);
    pids[2*i+1]=sys_createProcess(inc,"inc", 3, args2);
    sys_unblock(pids[2*i]);
    sys_unblock(pids[2*i+1]);
  }
  for(i = 0; i < 2*TOTAL_PAIR_PROCESSES; i++) sys_wait(pids[i]);
  sys_exit(0);
}

void test_no_sync(){
  uint64_t i;

  global = 100;


  printf("CREATING PROCESSES...(WITHOUT SEM)\n");
 
  char * args1[]={"0", "1",TESTS_QTY};
  char * args2[]={"0","-1", TESTS_QTY};

  unsigned long long pids[TOTAL_PAIR_PROCESSES*2];
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    pids[2*i]=sys_createProcess(inc,"inc", 3, args1);
    pids[2*i+1]=sys_createProcess(inc,"inc", 3, args2);
    sys_unblock(pids[2*i]);
    sys_unblock(pids[2*i+1]);
  }
  for(i = 0; i < 2*TOTAL_PAIR_PROCESSES; i++) sys_wait(pids[i]);
  sys_exit(0);
}


