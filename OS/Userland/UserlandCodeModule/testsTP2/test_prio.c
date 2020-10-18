#include <stdint.h>
#include <stdlib.h>
#include <syscalls.h>

#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice


void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void endless_loop(){
  uint64_t pid = sys_getPid();

  while(1){
    printf("%d ",pid);
    //sys_yield();
    bussy_wait(MINOR_WAIT);
  }
  
}

#define TOTAL_PROCESSES 3

void test_prio(){

  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++){
    pids[i]=sys_createProcess(endless_loop,"endless_loop",0,0);
    sys_unblock(pids[i]);
  }

   //sys_yield();
  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        sys_nice(pids[i], 1); //lowest priority 
        break;
      case 1:
        sys_nice(pids[i], 3); //medium priority
        break;
      case 2:
        sys_nice(pids[i], 5); //highest priority
        break;
    }
  }
  // sys_yield();
  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    sys_block(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        sys_nice(pids[i], 3); //medium priority 
        break;
      case 1:
        sys_nice(pids[i], 3); //medium priority
        break;
      case 2:
        sys_nice(pids[i], 3); //medium priority
        break;
    }
  }

  printf("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    sys_unblock(pids[i]);
  bussy_wait(WAIT);
  printf("\nKILLING...\n");
  sys_yield();
  for(i = 0; i < TOTAL_PROCESSES; i++)
    sys_kill(pids[i]);
  sys_yield();
  sys_exit(0);
}


