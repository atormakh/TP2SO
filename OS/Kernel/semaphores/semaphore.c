#include<semaphore.h>
#include<scheduler.h>
#include<list.h>
#include<lib.h>
#include<ipc.h>

int _xchg(int * addr,int value);

List * semaphores;
int lock = 0;

int cmpSems(void * elem1, void * elem2){

    return strcmp(((Sem *)elem1 )->id,((Sem *)elem2)->id);

}


void initSemaphores(){
    semaphores = newList(cmpSems);
}

void openSem(char * semId, int value){
    acquire(&lock);
    Sem * sem, * semSearcher;
    sem = semSearcher =(Sem *) c_alloc(sizeof(Sem));
    strcpy(sem->id, semId);
    sem =(Sem *) get(semaphores, semSearcher);
    if(sem == NULL){
        sem = semSearcher;
        createMotive(sem);
        push(semaphores, sem);
        sem->value=value;      
    }
    sem->refs++;
    release(&lock);
}

void semPost(char * semId){
    Sem * semID;
    semID = (Sem *) c_alloc(sizeof(Sem));
    strcpy(semID->id, semId);
    Sem * sem = (Sem *)get(semaphores, semID);
    if(sem == NULL){
        return;
    }
    acquire(&sem->lock);
    sem->value++;
    awakeAll(sem); 
    release(&sem->lock);
}

void semWait(char * semId){
    Sem * semID;
    semID = (Sem *) c_alloc(sizeof(Sem));
    strcpy(semID->id, semId);
    Sem * sem = (Sem *)get(semaphores, semID);
    if(sem == NULL){
        return;
    }
    acquire(&sem->lock);
    while(sem->value<=0){
        
        release(&sem->lock);
        block(sem, getCurrentProc()->pid);
        yield();
        acquire(&sem->lock);
    }
    sem->value--;
    release(&sem->lock);
}
void closeSem(char * semId){
    acquire(&lock);
    Sem * semID;
    semID = (Sem *) c_alloc(sizeof(Sem));
    strcpy(semID->id, semId);
    Sem * sem = (Sem *)get(semaphores, semID);

    if(sem == NULL){
        release(&lock);
        return;
    }else{
        sem->refs--;
    }
    if(sem->refs == 0){
        closeMotive(sem);
        m_free(sem);
    }
    
    release(&lock);
}

void acquire(int * lock){
    while(_xchg(lock,1)!=0)yield(); //en monocore tiene sentido el yield
}

void release(int * lock){
    _xchg(lock, 0);
}