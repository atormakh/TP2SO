GLOBAL getHour
GLOBAL getMin
GLOBAL getSecs
getHour:
    push rbp
    mov rbp,rsp
    mov al,0x04
    out 70h,al
    in al,71h
    pop rbp
    ret

getMin:
    push rbp
    mov rbp,rsp
    mov al,0x02
    out 70h,al
    in al,71h
    pop rbp
    ret
getSecs:
    push rbp
    mov rbp,rsp
    mov al,0x0
    out 70h,al
    in al,71h
    pop rbp
    ret