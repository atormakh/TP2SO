#include <screenDrawer.h>
#include <stdlib.h>
#include <syscalls.h>
#define SIZE 2048

char out[SIZE];
unsigned int index=0;

tabStruct tab0 = {"shell@covinux:>",0,0,0,{0},0,10,{10,120,1014,750},0,0};

tabStruct * tabs[]={&tab0};

colorStruct colorGrey={200,200,200},colorOrange={0,0,255},colorBlack={0,0,0};
rect tab0_border;


rect * tab_borders[]={&tab0_border};
int focus=0;

int screenDrawer(){
    drawPixMaps();
    setupBorders();
    initTabs();
    sys_sleep(2);
    int n=0;
    sys_readPipe(0,out,SIZE,&n);
    while( n){
        index=0;
        writeOnTab(&tab0);
        sys_readPipe(0,out,SIZE-1,&n);
        out[n]=0;
    }
}

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

void writeOnTab( tabStruct *tab){  
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

void setupBorders(){
	tab0_border.xi = tab0.currentScreen.xi-BORDER;
	tab0_border.xf = tab0.currentScreen.xf+BORDER;
	tab0_border.yi = tab0.currentScreen.yi-BORDER;
	tab0_border.yf = tab0.currentScreen.yf+BORDER;
	tab0_border.fill=0;
	tab0_border.border=3;
	tab0_border.c = colorGrey;

	

	tab_borders[focus]->c = colorOrange;
	sys_drawRect(&tab0_border);
}

void drawPixMaps(){
	sys_drawBitmap(400,15,itbaLogo_xpm);
	sys_drawBitmap(600,0,shellLogo_xpm);
}

void initTabs(){
	for(int i=0;i<NUM_TABS;i++){
		initTab(tabs[i]);

	}
}

void initTab(tabStruct * tab){
	//que arranquen las pantallas con username escrito
	tab->current=0;
	tab->offsetCurrent = tab->current+1;
	tab->inIndex=0;
}

