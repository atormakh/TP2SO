#include<semaphore.h>
#include<scheduler.h>

int xchg(int value, int * addr);

Sem * createSem(int value){
    Sem * sem = c_alloc(sizeof(Sem));
    createMotive(sem);
    sem->value=value;
    return sem;
}
void semPost(Sem * sem){
   
    xchg(sem->value+1, &sem->value);
    awake(sem); 


}
void semWait(Sem * sem){
    _cli();
    // if(sem->value>0) sem->value-- queremos que esto sea atomico
    while(xchg(sem->value-1, &sem->value)<=0){
        block(sem, getCurrentProc()->pid);
        _sti();
        yield();
    }

}
void closeSem(Sem * sem){
    closeMotive(sem);
    m_free(sem);
}