#include<files.h>
static char * out;
unsigned int index=0;
unsigned int size;
void mapstdout(char * buffer, int s){
    out=buffer;
    size=s;
    index=0;
}

void write(char * buffer, int count){
     
    for(int i=0;i<count;i++){
         out[index%size]=buffer[i];
         index++;
    }
    out[index%size]=0;
}