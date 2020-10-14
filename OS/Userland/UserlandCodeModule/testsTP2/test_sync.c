#include <stdint.h>
#include <stdio.h>
#include<syscalls.h>

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"
#define TESTS_QTY 10

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  sys_yield();
  *p = aux;
}

void inc(int argc, char * args[]){
  

  uint64_t sem = args[0];
  int value = args[1]; 
  uint64_t N = args[2];
  uint64_t i;
 
  void * semaphore = args[3]; //borrar
  
  /*if(sem)sys_createSem(1,&semaphore);
  if (sem  && !semaphore ){
    printf("ERROR OPENING SEM\n");
    return;
  }
*/
  
  for (i = 0; i < N; i++){

    if (sem) sys_semWait(semaphore);
    slowInc(&global, value); 
    if (sem) sys_semPost(semaphore);
    sys_yield();
    sys_yield();
    sys_yield();
    sys_yield();
  }

  
  //if (sem) sys_closeSem(semaphore);
  
  printf("Final value: %d\n", global);
  sys_yield();
  sys_exit(0);
}

void test_sync(){
  uint64_t i;

  global = 100;

  printf("CREATING PROCESSES...(WITH SEM)\n");
  
  unsigned long long pid1;
  unsigned long long pid2;
  void * semaphore; //bnorrar
  sys_createSem(1,&semaphore); //borrar

  char * args1[]={1, 1, TESTS_QTY,semaphore};
  char * args2[]={1, -1, TESTS_QTY,semaphore};

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    sys_createProcess(inc, 3, args1, &pid1);
    sys_createProcess(inc, 3, args2, &pid2);
  }
  sys_exit(0);
  //while(1);
}

void test_no_sync(){
  uint64_t i;

  global = 100;


  printf("CREATING PROCESSES...(WITHOUT SEM)\n");
 
  unsigned long long pid1;
  unsigned long long pid2;
  void * semaphore; //bnorrar
  sys_createSem(1,&semaphore); //borrar

  char * args1[]={0, 1, TESTS_QTY,semaphore};
  char * args2[]={0, -1, TESTS_QTY,semaphore};

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    sys_createProcess(inc, 3, args1, &pid1);
    sys_createProcess(inc, 3, args2, &pid2);
  }

  sys_exit(0);
}


