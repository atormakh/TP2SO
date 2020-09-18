#include <syscalls.h>
#include <stdint.h>
#include <setJmp.h>
#include <windows.h>
#include <errors.h>
#include <stdlib.h>
#include "defs.h"
#include <calculator.h>
#include <shell.h>
#include "itbaLogo.xpm"
#include "shellLogo.xpm"
#include "calcLogo.xpm"

#define RADIUS 1
unsigned char focus = 0;
#define resX  1024
#define resY  768
#define BORDER 10
#define MOVE 50
#define SCALE 30

void exGenericHandler(errorStruct * error);
void initTabs();
void initTab(tabStruct * tab);

tabStruct tab0 = {"calc@covinux:>",evaluator,inControllerTab0,exGenericHandler,{0},0,10,{10,200,500,750},0,0};
tabStruct tab1 = {"shell@covinux:>",shell,inControllerTab1,exGenericHandler,{0},0,10,{524,200,1014,750},0,0};

tabStruct * tabs[]={&tab0,&tab1};

colorStruct colorGrey={200,200,200},colorOrange={255,125,0},colorBlack={0,0,0};
rect tab0_border;
rect tab1_border;

rect * tab_borders[]={&tab0_border,&tab1_border};
registerEnv env;

void setupBorders(){
	tab0_border.xi = tab0.currentScreen.xi-BORDER;
	tab0_border.xf = tab0.currentScreen.xf+BORDER;
	tab0_border.yi = tab0.currentScreen.yi-BORDER;
	tab0_border.yf = tab0.currentScreen.yf+BORDER;
	tab0_border.fill=0;
	tab0_border.border=3;
	tab0_border.c = colorGrey;

	

	tab1_border.xi = tab1.currentScreen.xi-BORDER;
	tab1_border.xf = tab1.currentScreen.xf+BORDER;
	tab1_border.yi = tab1.currentScreen.yi-BORDER;
	tab1_border.yf = tab1.currentScreen.yf+BORDER;
	tab1_border.fill=0;
	tab1_border.border=3;
	tab1_border.c = colorGrey;

	tab_borders[focus]->c = colorOrange;
	sys_drawRect(&tab1_border);
	sys_drawRect(&tab0_border);
}

typedef struct moves{
	int up, down,left, right;
}moves;

void drawPixMaps(){
	sys_drawBitmap(0,0,itbaLogo_xpm);
	sys_drawBitmap(700,76,shellLogo_xpm);
	sys_drawBitmap(200,76,calcLogo_xpm);
}

int main() {
	createstdout();
	setupBorders();
	drawPixMaps();
			
	int c=0;
	

	sys_setExceptionHandler(0,exGenericHandler); //esta parte siempre justo antes del while
	sys_setExceptionHandler(6,exGenericHandler);
	int entry=setjmp(&env)-1;
	if(entry == -1){
		initTabs();
	}

	while(1){
		while((c=getChar()) !='\n'){
			if(c=='\t'){
				tab_borders[focus]->c=colorGrey;
				sys_drawRect(tab_borders[focus]);
				focus = (focus+1)%NUM_TABS;
				tab_borders[focus]->c=colorOrange;
				sys_drawRect(tab_borders[focus]);

			}else if(c==208){
				eraseTab(tabs[focus]);
				initTab(tabs[focus]);			
			}else if(c>=200 && c<=207){
				drawPixMaps();
				eraseTab(tabs[focus]);
				tab_borders[focus]->c=colorBlack;
				sys_drawRect(tab_borders[focus]);

				switch (c){
					case 200:  // para arriba
						tabs[focus]->currentScreen.yi-=MOVE;
						tabs[focus]->currentScreen.yf-=MOVE;
						break;
					case 201: // para abajo
						tabs[focus]->currentScreen.yi+=MOVE;
						tabs[focus]->currentScreen.yf+=MOVE;
					break;
					case 202: // para izquierda
						tabs[focus]->currentScreen.xi-=MOVE;
						tabs[focus]->currentScreen.xf-=MOVE;
					break;
					case 203: // para derecha
						tabs[focus]->currentScreen.xi+=MOVE;
						tabs[focus]->currentScreen.xf+=MOVE;
					break;
				case 204:  // agrandar vertical
					tabs[focus]->currentScreen.yi-=SCALE;
					break;
				case 205: //  achicar vertical
					if(tabs[focus]->currentScreen.yi+4* SCALE < tabs[focus]->currentScreen.yf)
						tabs[focus]->currentScreen.yi+=SCALE;
					break;
				case 206: // agrandar horizontal
					tabs[focus]->currentScreen.xi-=SCALE;
					break;
				case 207: // achicar horizontal
					if(tabs[focus]->currentScreen.xi+4*SCALE < tabs[focus]->currentScreen.xf)
						tabs[focus]->currentScreen.xi+=SCALE;
					break;
				
				}
				setupBorders();
				initTabs();
				
				sys_drawRect(tab_borders[focus]);

			}else{
				tabs[focus]->inController(c);
			}
		}

		tabStruct * currentTab = tabs[focus];

		putchar('\n');
		currentTab->run(currentTab->in);
		putchar('\n');
		puts(tabs[focus]->name);
		flushstdout(currentTab);
		currentTab->offsetCurrent = currentTab->current+1;
		currentTab->inIndex=0;
		currentTab->in[0]=0;
	}

	return 0;
}

void initTabs(){
	for(int i=0;i<NUM_TABS;i++){
		initTab(tabs[i]);

	}
}

void initTab(tabStruct * tab){
	//que arranquen las pantallas con username escrito
	tab->current=0;
	puts(tab->name);
	flushstdout(tab);
	tab->offsetCurrent = tab->current+1;
	tab->inIndex=0;

	
}


void runGenerico(char * in,char * out){
	return;
}


void genericInController(int c, tabStruct * tab){
	
	if(c==128){
		while(tab->inIndex>0){
			genericInController(8,tab);
		}
	}
	else{
		if(c==8){
			if(tab->inIndex>0){
				tab->inIndex--;
				tab->in[tab->inIndex]=0;
			}
		}
		else{
			tab->in[tab->inIndex++] = c;
			tab->in[tab->inIndex]=0;
		}

		putchar(c);
		if(c!=0)flushstdout(tab);	
	}
}

void inControllerTab1(int c){
	genericInController(c,&tab1);
	
}

void inControllerTab0(int c){

	if((c>='0' && c<='9') || c=='.' || c==',' || c=='+' || c=='-' || c=='/' || c=='*' || c=='(' || c==')'|| c==8 || c==128)
		genericInController(c,&tab0);
	
}


void exGenericHandler(errorStruct * error){
	
	switch (error->errorCode)
	{
	case 0:
		puts("Excepcion de division por cero \n");
		break;
	
	case 6:
		puts("Excepcion de codigo de operacion invalido \n");
		break;
	}

	char * registersNames[] = {"r15","r14","r13","r12","r11","r10","r9","r8","rsi","rdi",\
                            "rbp","rdx","rcx","rbx","rax","rsp","rip","cs","flags"};
	uint64_t * registers =(uint64_t *)&(error->registers);
    puts("registros \n");
    
    for(int i = 0 ; i < 19 ; i++){
		printf("%s    %h \n",registersNames[i],registers[i]);
    
    }	
	puts(tabs[focus]->name);
	flushstdout(tabs[focus]);
	tabs[focus]->offsetCurrent = tabs[focus]->current+1;
	tabs[focus]->inIndex=0;
	longjmp(&env,error->errorCode+1);

}
