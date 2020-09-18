#include <systemInfo.h>

unsigned char getHour();
unsigned char getMin();
unsigned char getSecs();

void localtime(char * buffer){
    char hour,min,secs;
    hour = getHour();
    min = getMin();
    secs = getSecs();
    buffer[0]=(hour>>4)+'0';
    buffer[1]=(hour & 0x0F)+'0';
    buffer[2]=':';
    buffer[3]=(min>>4)+'0';
    buffer[4]=(min & 0x0F)+'0';
    buffer[5]=':';
    buffer[6]=(secs>>4)+'0';
    buffer[7]=(secs & 0x0F)+'0';
    buffer[8]=' ';
    buffer[9]='G';
    buffer[10]='M';
    buffer[11]='T';
    buffer[12]=0;

}
