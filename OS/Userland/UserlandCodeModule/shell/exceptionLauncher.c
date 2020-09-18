

void divisionbyzero(){
    int a=0;
    int b=1/a;
    a=b; // only for warning purposes
}

void invalidopcode(){
    void  (* func)(void)=(void(*)(void))"esto no es un opcode";
    func();
}