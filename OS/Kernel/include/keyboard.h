#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <registersArgs.h>
#include <stdint.h>

#define ALT_L 56
#define SHIFT_L 42
#define SHIFT_R 54
#define BLOCK_MAYUS 58
#define CTRL_L 29
#define RELEASED 128

void keyboard_handler(registerArgs * regs);
void readKeyboard(char * buffer, int count,int * amount);
char getKey();
char hasKey();
char getPort64();
void retrieveRegs(registerArgs * args, int * flag);

#endif