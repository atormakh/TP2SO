#ifndef IPC_H
#define IPC_H

void writePipe(int fd,char * buffer,int maxWrite);
void readPipe(int fd,char * buffer,int maxRead,int * qty);

#endif