#ifndef SCREEN_DRAWER_H_
#define SCREEN_DRAWER_H_
#include <errors.h>

#define OUT_LENGTH 2000
#define LINES_LENGTH 50
#define IN_LENGTH 512
#define BORDER 10
#define NUM_TABS 1

typedef struct colorStruct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;

} colorStruct;

typedef struct rect{
    int xi,yi;
    int xf, yf;
    unsigned char fill;
    unsigned int border;
    colorStruct c;
    
}rect;


typedef struct screen{
	int xi,yi,xf,yf;
}screen;

typedef struct tabStruct{
	char * name;
	void  (* run)(char *);
	void (* inController) (int);
	void (* exceptionsHandler)(errorStruct *);
	char in[IN_LENGTH];
	int inIndex;
	int px;
	screen currentScreen;
	int offsetCurrent;
	int current;
}tabStruct;

void setupBorders();
void drawPixMaps();
void initTabs();
void screenDrawer();
void writeOnTab(tabStruct * );
void eraseTab(tabStruct * );
void initTab(tabStruct * tab);


#endif