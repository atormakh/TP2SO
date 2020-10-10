void sys_drawCharacter(int x, int y, int px, char letter);
void sys_drawBitmap(int,int,char ** bitmap);
void sys_readErrors(void * error); //borrar?
void sys_readKeyboard(char * buffer,int count,int * amount);
void sys_setExceptionHandler(int exception, void * func); //borrar?
void sys_scroll(int xi, int yi, int xf, int yf, int px);
void sys_retrieveReg(void * reg, int * flag); // borrar?
void sys_cputemp(int *); // borrar?
void sys_localtime(char * buffer); //borrar?
void sys_drawRect(void * rectangle);
void sys_mapstdout(void * buffer,int size); //borrar
void sys_write(void * buffer, int count); //hay que editarlo para que quede bien
void sys_createProcess(void * func, int argc, char * args[], unsigned long long * pid);
void sys_yield();
void sys_exit(int status);
void sys_ps(char * buffer);
void sys_sleep(unsigned int seconds);
void sys_pipe(unsigned long yter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead );
void sys_readPipe(int fd,char * buffer, int n, int * qty);
void sys_writePipe(int fd,char * buffer, int n);


void sys_getPid(int * pid);
