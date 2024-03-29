#include<semaphore.h>
#include<scheduler.h>
#include<list.h>
#include<lib.h>
#include<ipc.h>

int _xchg(int * addr,int value);

List * semaphores;
int lock = 0;

void initSemaphores(){
    semaphores = newList();
}

void openSem(char * semId, int value){
    acquire(&lock);
    Sem * sem;
    sem =(Sem *) get(semaphores, strHash(semId));
    if(sem == NULL){
        sem = (Sem *) c_alloc(sizeof(Sem));
        if(sem == NULL)
            return;
        strcpy(sem->id, semId);
        createMotive(sem);
        push(semaphores, sem,strHash(semId));
        sem->value=value;      
    }
    sem->refs++;
    release(&lock);
}

void semPost(char * semId){
    Sem * sem = (Sem *)get(semaphores, strHash(semId));
    if(sem == NULL){
        return;
    }
    acquire(&sem->lock);
    sem->value++;
    awakeAll(sem); 
    release(&sem->lock);
}

void semWait(char * semId){
    Sem * sem = (Sem *)get(semaphores, strHash(semId));
    if(sem == NULL){
        return;
    }
    acquire(&sem->lock);
    while(sem->value<=0){
        
        release(&sem->lock);
        blockMotive(sem, getCurrentProc()->pid);
        yield();
        acquire(&sem->lock);
    }
    sem->value--;
    release(&sem->lock);
}
void closeSem(char * semId){
    acquire(&lock);;
    Sem * sem = (Sem *)get(semaphores, strHash(semId));

    if(sem == NULL){
        release(&lock);
        return;
    }else{
        sem->refs--;
    }
    if(sem->refs == 0){
        closeMotive(sem);
        remove(semaphores,(unsigned long long)strHash(sem->id));
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

void semsInfo(char * buffer){
    buffer+=strcpy(buffer, "SEM_ID | value | blocked  \n");
    iterator(semaphores);
    while(hasNext(semaphores)){
        Sem * sem = getNext(semaphores);
        buffer += strcpy(buffer, sem->id);
        buffer += strcpy(buffer, "  |  ");
        buffer += intToString( sem->value, buffer);
        buffer += strcpy(buffer, "  |  ");

        buffer+=printProcsInMotive(buffer, getMotive((void *)strHash(sem->id)));
        
        *buffer++='\n';
    }
    *buffer++=0;    

}