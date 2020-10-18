GLOBAL sys_readKeyboard
GLOBAL sys_drawCharacter
GLOBAL sys_drawBitmap
GLOBAL sys_scroll
GLOBAL sys_drawRect
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
GLOBAL sys_nice
GLOBAL sys_getPid
GLOBAL sys_kill
GLOBAL sys_block
GLOBAL sys_unblock
GLOBAL sys_wait
GLOBAL sys_m_alloc
GLOBAL sys_m_free
GLOBAL sys_c_alloc

sys_readKeyboard:  ;char* buffer, int count
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

sys_scroll: ; int xi, int yi, int xf, int yf, int px
	push rbp
	mov rbp, rsp
	mov rax, 3
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_drawRect:  ;rect * rectangle
	push rbp
	mov rbp,rsp
	mov rax, 4
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_createProcess:  ;void * func, char * name, int argc, char * args[]
	push rbp
	mov rbp,rsp
	mov rax, 5
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_yield:  ;
	push rbp
	mov rbp,rsp
	mov rax, 6
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_exit:  ; int status
	push rbp
	mov rbp,rsp
	mov rax, 7
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_ps:  ;char * buffer
	push rbp
	mov rbp,rsp
	mov rax, 8
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_sleep:  ;unsigned int seconds
	push rbp
	mov rbp,rsp
	mov rax, 9
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_pipe:  ;int pipe(unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead )sys_pipe(int pid1,, int n)
	push rbp
	mov rbp,rsp
	mov rax, 10
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_readPipe:  ;sys_read(int fd,char * buffer, int n)
	push rbp
	mov rbp,rsp
	mov rax, 11
	int 80h
	mov rsp, rbp
	pop rbp
	ret
sys_writePipe:  ;sys_write(int fd,char * buffer, int n)
	push rbp
	mov rbp,rsp
	mov rax, 12
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_openSem:  ;sys_createSem(int value, void ** sem)
	push rbp
	mov rbp,rsp
	mov rax, 13
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_semPost:  ;sys_semPost(void * sem)
	push rbp
	mov rbp,rsp
	mov rax, 14
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_semWait:  ;sys_semWait(void * sem)
	push rbp
	mov rbp,rsp
	mov rax, 15
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_closeSem:  ;sys_closeSem(void * sem)
	push rbp
	mov rbp,rsp
	mov rax, 16
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_nice: 		;sys_nice(unsigned long long pid, unsigned int priority)
	push rbp
	mov rbp,rsp
	mov rax, 17
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_getPid: 		;sys_getPid(unsigned long long * pid)
	push rbp
	mov rbp,rsp
	mov rax, 18
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_kill: 		;sys_kill(unsigned long long pid)
	push rbp
	mov rbp,rsp
	mov rax, 19
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_block: 		;sys_block(unsigned long long pid,)
	push rbp
	mov rbp,rsp
	mov rax, 20
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_unblock: 		;sys_unblock(unsigned long long pid)
	push rbp
	mov rbp,rsp
	mov rax, 21
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_wait: 		;sys_wait(unsigned long long pid)
	push rbp
	mov rbp,rsp
	mov rax, 22
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_m_alloc:
	push rbp
	mov rbp,rsp
	mov rax, 23
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_m_free:
	push rbp
	mov rbp,rsp
	mov rax, 24
	int 80h
	mov rsp, rbp
	pop rbp
	ret

sys_c_alloc:
	push rbp
	mov rbp,rsp
	mov rax, 25
	int 80h
	mov rsp, rbp
	pop rbp
	ret