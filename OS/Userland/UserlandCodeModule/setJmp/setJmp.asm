EXTERN printNum
GLOBAL setjmp
GLOBAL longjmp

setjmp:
    pop rdx
    mov [rdi], r15
    add rdi, 8
    mov [rdi], r14
    add rdi, 8
    mov [rdi], r13
    add rdi, 8
    mov [rdi], r12
    add rdi, 8
    mov [rdi], rbp
    add rdi, 8
    mov [rdi], rdx
    add rdi, 8
    mov [rdi], rbx
    add rdi, 8
    pushfq 
    pop rax
    mov [rdi], rax
    add rdi, 8
    mov [rdi], rsp
    add rdi, 8
    mov rax, 0
    mov qword [rdi], $
    push rdx
    ret

longjmp:
    mov r15, [rdi]
    add rdi, 8
    mov r14, [rdi]
    add rdi, 8
    mov r13, [rdi]
    add rdi, 8
    mov r12, [rdi]
    add rdi, 8
    mov rbp, [rdi]
    add rdi, 8
    mov rdx, [rdi]
    add rdi, 8
    mov rbx, [rdi]
    add rdi, 8
    mov rax, [rdi]
    push rax
    popfq
    add rdi, 8
    mov rsp, [rdi]
    add rdi, 8
    mov rcx, [rdi]
    mov rax, rsi
    jmp rcx
    