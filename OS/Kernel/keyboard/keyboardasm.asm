GLOBAL getKey
GLOBAL hasKey
GLOBAL getPort64

getKey:
    in al,60h
    ret
hasKey:
    in al,64h
    and al,1
    ret
getPort64:
    in al,64h
    ret