#include <stdint.h>
#include "fonts.h"
#include "xPixMap.h"
#include <video.h>

struct vbe_mode_info_structure{
    uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;			// deprecated
    uint8_t window_b;			// deprecated
    uint16_t granularity;		// deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;			// number of bytes per horizontal line
    uint16_t width;			// width in pixels
    uint16_t height;			// height in pixels
    uint8_t w_char;			// unused...
    uint8_t y_char;			// ...
    uint8_t planes;
    uint8_t bpp;			// bits per pixel in this mode
    uint8_t banks;			// deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint64_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__((packed));

void setPixel(int x, int y, colorStruct c);
char getBit(unsigned char index,int x, int y);

struct vbe_mode_info_structure * screen_info = (struct vbe_mode_info_structure * )0x5C00;

void segmentRowCopy(int dest, int src, int start, int end){
    char * framebuffer=(char *)screen_info->framebuffer;

    int absSrc;
    int absDest;

    char * pixelSrc;
    char * pixelDest;

    for(int i = start; i<=end;i++){
        absSrc=3*(i+src*screen_info->width);
        absDest=3*(i+dest*screen_info->width);
        pixelSrc = framebuffer + absSrc;
        pixelDest = framebuffer + absDest;
        pixelDest[0]=pixelSrc[0];
        pixelDest[1]=pixelSrc[1];
        pixelDest[2]=pixelSrc[2];
    }
}

void scroll(int xi, int yi, int xf, int yf, int px){
    for(int i=yi+px; i<= yf;i++){
        segmentRowCopy(i-px,i,xi,xf);
    }
}

void drawCharacter(int x, int y, int px, char letter){
    colorStruct black = {0,0,0};
    colorStruct green = {0,200,0};
    double scale=1.0f*px/letter_width;
    for(int j = 0; j < letter_height*scale;j++){
        for(int i = 0; i < letter_width*scale;i++){
            setPixel(i+x,j+y,getBit(letter,i/scale,j/scale)?green:black);
        }
	}   
}

void drawBitmap(int x, int y, char * pixmap[]){ // max 16 colors support
    infoPixelMap pixelMap;
    loadPixelMap(&pixelMap,pixmap);
    //printNum(10);
    //printNum(pixelMap.values.height);
    for(int j=0; j<pixelMap.values.height;j++){
        for(int i=0;i<pixelMap.values.width;i++){
            setPixel(i+x,j+y,getColor(i,j,&pixelMap));
        }
    }

}

void setPixel(int x, int y, colorStruct c){
    char * framebuffer =(char *)screen_info->framebuffer;
    int yCycles = y<0? 768 * ((-y/768)+1):0;
    int xCycles = x<0? 1024 * ((-x/1024)+1):0;
    int absolute=3*((x+xCycles)%1024+((y+yCycles)%768)*screen_info->width);
    char * pixel = framebuffer+absolute;
    pixel[0]=c.blue;
    pixel[1]=c.green;
    pixel[2]=c.red;
    
}

char getBit(unsigned char index,int x, int y){
  	int absolute = x + y * letter_bytes*8;
  	int pos = absolute/8;
  	int bit = absolute%8;
  	return (bitmap[index-OFFSET][pos]>>(bit))&0x01;	
}
void drawLine(int xi,int yi,int xf,int yf,colorStruct c){
    if(xi==xf){
        for(int i = yi ; i < yf ; i++){
            setPixel(xi,i,c);
        }
        return;
    }
    int m = (yf-yi)/(xf-xi);
    int b = yi - m*xi;
    for(int i = xi ; i < xf ; i++){
          setPixel(i,m*i+b,c);  
    }
}

void drawRect(rect * rectangle){
    
    if(rectangle->fill){
        for(int j = rectangle->yi;j<rectangle->yf;j++){
        
           drawLine(rectangle->xi,j,rectangle->xf,j,rectangle->c);
            
        }
        return;
    }
    
    // borde vertical izquierdo
    for(int i=rectangle->xi;i<rectangle->xi+rectangle->border;i++){
        drawLine(i,rectangle->yi,i,rectangle->yf,rectangle->c);
    }

    //borde vertical derecho
    for(int i=rectangle->xf-rectangle->border;i<rectangle->xf;i++){
        drawLine(i,rectangle->yi,i,rectangle->yf,rectangle->c);
    }

    //borde horizontal superior
    for(int i=rectangle->yi;i<rectangle->yi+rectangle->border;i++){
        drawLine(rectangle->xi,i,rectangle->xf,i,rectangle->c);
    }

    //borde horizontal inferior
    for(int i=rectangle->yf-rectangle->border;i<rectangle->yf;i++){
        drawLine(rectangle->xi,i,rectangle->xf,i,rectangle->c);
    }
}





