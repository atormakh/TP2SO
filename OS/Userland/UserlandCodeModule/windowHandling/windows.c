#include <windows.h>
#include <stdlib.h>
#include <syscalls.h>

#define SIZE 2048
char out[SIZE];
unsigned int index=0;
void eraseTab(tabStruct * tab){
    rect eraser ={
					tab->currentScreen.xi,
					tab->currentScreen.yi,
					tab->currentScreen.xf,
					tab->currentScreen.yf,
					1,
					0,
					{0,0,0}};
    sys_drawRect(&eraser);
}

void createstdout(){
	sys_mapstdout(out, SIZE);
}

void flushstdout( tabStruct *tab)
{  
    int height = tab->currentScreen.yf - tab->currentScreen.yi;
    int width = tab->currentScreen.xf - tab->currentScreen.xi;
    int px = tab->px;
    int lettersPerLine = width / px; //cambiar a syscall getResolution
    int lineHeight = 2*px +2;
    int totalLines = height/(lineHeight);
    for (; out[index%SIZE] != 0; index++){
        if (out[index%SIZE] == 8){
            if(tab->current >= tab->offsetCurrent){
                out[index%SIZE] = 32;
                tab->current--;
                int x_offset = tab->currentScreen.xi + px * ((tab->current) % lettersPerLine);
                int y_offset = tab->currentScreen.yi + (lineHeight) * ((tab->current) / lettersPerLine);
                sys_drawCharacter(x_offset, y_offset, px, out[index%SIZE]);
            }
        } else {
            if(out[index%SIZE]=='\n'){
                tab->current+=lettersPerLine- tab->current%lettersPerLine;

            }
            else{
                int x_offset = tab->currentScreen.xi + px * ((tab->current) % lettersPerLine);
                int y_offset = tab->currentScreen.yi + (lineHeight) * ((tab->current)/ lettersPerLine);
                sys_drawCharacter(x_offset, y_offset, px, out[index%SIZE]);
                tab->current++;
            }
            if((tab->current) / lettersPerLine>=(totalLines-1)){
                sys_scroll(tab->currentScreen.xi, tab->currentScreen.yi, \
                            tab->currentScreen.xf, tab->currentScreen.yf, \
                            lineHeight);
                tab->current-=lettersPerLine;
                tab->offsetCurrent-=lettersPerLine;
            }
        }
    }
}