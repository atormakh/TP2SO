GLOBAL yield
extern dec_tick


yield:
    int 20h
    call dec_tick
    ret
