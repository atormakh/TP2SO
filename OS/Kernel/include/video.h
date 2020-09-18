#ifndef VIDEO_H
#define VIDEO_H
#include <stdint.h>

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

void drawCharacter(int x, int y, int px, char letter);
void drawBitmap(int x, int y, char * bitmap[]);
void drawRect(rect * rectangle);
void scroll(int xi, int yi, int xf, int yf, int px);
#endif