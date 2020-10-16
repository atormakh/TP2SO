GLOBAL sys_drawCharacter
GLOBAL sys_readErrors
GLOBAL sys_drawBitmap
GLOBAL sys_readKeyboard
GLOBAL sys_setExceptionHandler
GLOBAL sys_scroll
GLOBAL sys_retrieveReg
GLOBAL sys_cputemp
GLOBAL sys_localtime
GLOBAL sys_drawRect
GLOBAL sys_mapstdout
GLOBAL sys_write
GLOBAL sys_createProcess
GLOBAL sys_yield
GLOBAL sys_exit
GLOBAL sys_ps
GLOBAL sys_sleep
GLOBAL sys_pipe
GLOBAL sys_readPipe
GLOBAL sys_writePipe
GLOBAL sys_openSem
GLOBAL sys_semPost
GLOBAL sys_semWait
GLOBAL sys_closeSem

sys_readKeyboard:  ;char* buffer, int count,int * amount
	push rbp
	mov rbp, rsp
	mov rax, 0
	int 80h
	mov rsp, rbp
	pop rbp
	ret


sys_drawCharacter: ;int x, int y, int px, char letter
    push rbp
    mov rbp,rsp
    mov rax, 1
    int 80h
    mov rsp,rbp
    pop rbp
    ret


sys_drawBitmap:  ;int x, int y, char * bitmap
	push rbp
	mov rbp, rsp
	mov rax, 2
	int 80h
	mov rsp, rbp
	pop rbp
	ret	


sys_cputemp:  ;double * temp
	push rbp
	mov rbp, rsp
	mov rax, 3
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_readErrors:  ;char* buffer
	push rbp
	mov rbp, rsp
	mov rax, 4
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_setExceptionHandler: ; int exception, void * func
	push rbp
	mov rbp, rsp
	mov rax, 5
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_scroll: ; int xi, int yi, int xf, int yf, int px
	push rbp
	mov rbp, rsp
	mov rax, 6
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_retrieveReg:  ;registerArgs* reg
	push rbp
	mov rbp,rsp
	mov rax, 7
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_localtime:  ;char * reg
	push rbp
	mov rbp,rsp
	mov rax, 8
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_drawRect:  ;rect * rectangle
	push rbp
	mov rbp,rsp
	mov rax, 9
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_mapstdout:  ;void * buffer
	push rbp
	mov rbp,rsp
	mov rax, 10
	int 80h
	mov rsp, rbp
	pop rbp
	ret
sys_write:  ;void * buffer, int count
	push rbp
	mov rbp,rsp
	mov rax, 11
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_createProcess:  ;void * func, int argc, char * args[]
	push rbp
	mov rbp,rsp
	mov rax, 12
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_yield:  ;
	push rbp
	mov rbp,rsp
	mov rax, 13
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_exit:  ; int status
	push rbp
	mov rbp,rsp
	mov rax, 14
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_ps:  ;char * buffer
	push rbp
	mov rbp,rsp
	mov rax, 15
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_sleep:  ;unsigned int seconds
	push rbp
	mov rbp,rsp
	mov rax, 16
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_pipe:  ;int pipe(unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead )sys_pipe(int pid1,, int n)
	push rbp
	mov rbp,rsp
	mov rax, 17
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_readPipe:  ;sys_read(int fd,char * buffer, int n, int * qty)
	push rbp
	mov rbp,rsp
	mov rax, 18
	int 80h
	mov rsp, rbp
	pop rbp
	ret
sys_writePipe:  ;sys_write(int fd,char * buffer, int n)
	push rbp
	mov rbp,rsp
	mov rax, 19
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_openSem:  ;sys_createSem(int value, void ** sem)
	push rbp
	mov rbp,rsp
	mov rax, 20
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_semPost:  ;sys_semPost(void * sem)
	push rbp
	mov rbp,rsp
	mov rax, 21
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_semWait:  ;sys_semWait(void * sem)
	push rbp
	mov rbp,rsp
	mov rax, 22
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_closeSem:  ;sys_closeSem(void * sem)
	push rbp
	mov rbp,rsp
	mov rax, 23
	int 80h
	mov rsp, rbp
	pop rbp
	ret