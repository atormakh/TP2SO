#include "xPixMap.h"

void split(char * string, char buffer[4][5], char splitter){
    unsigned char index = 0;
    unsigned char actual=0;
    while(*string!=0){
        if(*string==splitter){
            buffer[actual][index]=0;
            actual++;
            index=0;
        }
        else
            buffer[actual][index++]=*string;
        string++;
    }
   
}

unsigned int atoi(char * string){
    unsigned int num=0;
    while(*string!=0){
        num=num*10+(*string -'0');
        string++;
    } 
    return num;
}

unsigned char hexToInt(char * hex ){
    uint8_t n1,n2;
    if(hex[0] >= 'A') n1=hex[0]-'A'+10;
    else n1=hex[0] - '0';
    if(hex[0] >= 'A') n2=hex[0]-'A'+10;
    else n2=hex[0] - '0';
    return ((n1<<4)&0xf0) | ((n2&0xf0));
}

void stringToColor(char * string, colorStruct * color){

    color->red = hexToInt(string);
    color->green = hexToInt(string+2);
    color->blue = hexToInt(string+4);
}



void loadPixelMap(infoPixelMap * info, char * pixmap[]){
    char valuesBuffer[4][5];
    //printString(pixmap[0]);
    split(*pixmap++,valuesBuffer,' ');
    //printString(valuesBuffer[0]);
    info->values.width=atoi(valuesBuffer[0]);
    info->values.height=atoi(valuesBuffer[1]);
    info->values.colors=atoi(valuesBuffer[2]);
    for(int i=0; i<info->values.colors;i++){
        char * data=*pixmap++;
        info->colors.chars[i]=data[0];
        stringToColor(data+5,&(info->colors.colors[i]));
    }  
    info->pixmap=pixmap;
}

colorStruct getColor(int x, int y, infoPixelMap * pixelmap){
    char ** pixmap=pixelmap->pixmap;
    char c = pixmap[y][x];
    int i=0;
    while(c!=pixelmap->colors.chars[i]) i++;
    return pixelmap->colors.colors[i];

    
}
