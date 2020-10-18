#include <stdlib.h>
#include <syscalls.h>

#define MAX_SIZE 1024


int isVowel(char c){
    return  c=='a' || c=='A' ||
            c=='e' || c=='E' ||
            c=='i' || c=='I' ||
            c=='o' || c=='O' ||
            c=='u' || c=='U'  ;
}

int filter(){
    char buffer[MAX_SIZE];
    int count;
    int index = 0;
    while((count=sys_readPipe(0,buffer,MAX_SIZE-1))){
        buffer[count]=0;
        for(int i=0;i<count;i++){
            if(!isVowel(buffer[i])){
                buffer[index]=buffer[i];
                index++;
            }
        }
        buffer[index]=0;
        index=0;
        puts(buffer);
    }
    sys_exit(0);
    return 0;
}