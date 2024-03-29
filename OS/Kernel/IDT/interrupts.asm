
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _syscallsHandler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN keyboard_handler
EXTERN timer_handler

EXTERN exceptionDispatcher
EXTERN syscallsDispatcher

SECTION .text

%macro pushState 0 
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax

%endmacro

;potential error stack struct ==== error,rip,cs,flags
%macro exceptionHandler 1
	;add rsp,8

	add rsp, 3*8
	mov [rsp-4*8],rsp
	sub rsp,4*8

	pushState
	mov rbp,rsp
	mov rsi,rbp
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher
	mov rsp,rbp
	popState

	add rsp,8
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	pushState
	mov rdi, rsp ; pasaje de parametro
	call timer_handler
	; signal pic EOI (End of Interrupt)
	mov rsp,rax
	mov al, 20h
	out 20h, al
	popState
	iretq

;Keyboard
_irq01Handler:
	add rsp, 3*8
	mov [rsp-4*8],rsp
	sub rsp,4*8

	pushState
	mov rdi, rsp
	call keyboard_handler
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	add rsp,8
	iretq



_syscallsHandler:;rax id, 
	
	
	add rsp, 3*8
	mov [rsp-4*8],rsp
	sub rsp,4*8

	pushState
	mov rbp,rsp
	push r9
	push r8
	push rcx
	push rdx
	push rsi
	push rdi
	mov rdi,rax
	mov rsi, rsp
	call syscallsDispatcher
	mov [returnValue],rax
	mov rsp,rbp
	popState
	mov rax,[returnValue]
	add rsp,8

	iretq

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1
	returnValue resb 8