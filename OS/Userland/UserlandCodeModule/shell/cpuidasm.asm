GLOBAL getcpuid

getcpuid: ;void cpuid(char * out);
    push rbp
    mov rbp,rsp
    push rbx
    mov rax,0
    cpuid
    mov [rdi],ebx
    mov [rdi+4],edx
    mov [rdi+8],ecx
    mov byte [rdi+12],32

    mov rax,80000002h
    cpuid
    mov [rdi+13],eax
    mov [rdi+17],ebx
    mov [rdi+21],ecx
    mov [rdi+25],edx
    
    mov rax,80000003h
    cpuid
    mov [rdi+29],eax
    mov [rdi+33],ebx
    mov [rdi+37],ecx
    mov [rdi+41],edx

    mov rax,80000004h
    cpuid
    mov [rdi+45],eax
    mov [rdi+49],ebx
    mov [rdi+53],ecx
    mov [rdi+57],edx


    pop rbx
    mov rsp,rbp
    pop rbp
    ret