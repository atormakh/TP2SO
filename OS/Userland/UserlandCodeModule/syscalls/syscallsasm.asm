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

