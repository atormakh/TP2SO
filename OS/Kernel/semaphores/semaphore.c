#include<semaphore.h>
#include<scheduler.h>

int _xchg(int * addr,int value);

Sem * createSem(int value){
    Sem * sem = c_alloc(sizeof(Sem));
    createMotive(sem);
    sem->value=value;
    return sem;
}
void semPost(Sem * sem){
    acquire(&sem->lock);
    sem->value++;
    awakeAll(sem); 
    release(&sem->lock);
    


}
void semWait(Sem * sem){
    
    acquire(&sem->lock);

    // if(sem->value>0){
    //     sem->value--;
    // }else{
    //     release(&sem->lock);
    //     block(sem, getCurrentProc()->pid);
    //     yield();
    //     acquire(&sem->lock);
    //     sem->value--;
    // }
    // release(&sem->lock);

    // if(sem->value>0) sem->value-- queremos que esto sea atomico
    
    while(sem->value<=0){
        
        release(&sem->lock);
        block(sem, getCurrentProc()->pid);
        yield();
        acquire(&sem->lock);
    }
    sem->value--;
    release(&sem->lock);
}
void closeSem(Sem * sem){
    closeMotive(sem);
    m_free(sem);
}

void acquire(int * lock){
    while(_xchg(lock,1)!=0)yield(); //en monocore tiene sentido el yield
}

void release(int * lock){
    _xchg(lock, 0);
}