GLOBAL xchg

xchg:
mov rax,rdi
xchg [rsi],rax
ret