GLOBAL cpuTemp

cpuTemp: ;void cpuTemp(int * res)
    push rbp
    mov rbp,rsp 

    mov ecx, 412
    rdmsr
    mov eax,882f0808h ; el valor retornado por una de nuestras computadoras; diff
    
    shr eax, 16
    mov sil,al
    and sil,7fh 

    mov ecx, 418
    rdmsr
    mov eax,690000h ; el valor retornado por una de nuestras computadoras ;temp limite

    shr eax, 16

    mov dword [rdi],0
    add [rdi],al
    sub [rdi],sil
    
    mov rsp, rbp
    pop rbp
