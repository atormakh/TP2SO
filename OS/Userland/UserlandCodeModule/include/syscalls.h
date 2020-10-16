void sys_drawCharacter(int x, int y, int px, char letter);
void sys_drawBitmap(int,int,char ** bitmap);
unsigned long long sys_readKeyboard(char * buffer,int count);
void sys_scroll(int xi, int yi, int xf, int yf, int px);
void sys_drawRect(void * rectangle);
unsigned long long sys_createProcess(void * func, int argc, char * args[]);
void sys_yield();
void sys_exit(int status);
void sys_ps(char * buffer);
void sys_sleep(unsigned int seconds);
void sys_pipe(unsigned long yter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead );
unsigned long long sys_readPipe(int fd,char * buffer, int n);
void sys_writePipe(int fd,char * buffer, int n);
void sys_openSem(char * sem_id,int value);
void sys_semPost(char * semId);
void sys_semWait(char * semId);
void sys_closeSem(char * semId);
void sys_nice(unsigned long long pid, unsigned int priority);
void sys_getPid(unsigned long long pid);
void sys_kill(unsigned long long pid);
void sys_block(unsigned long long pid);
void sys_unblock(unsigned long long pid);
void sys_wait(unsigned long long pid);



