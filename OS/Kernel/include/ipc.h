#ifndef IPC_H
#define IPC_H

void writePipe(int fd,char * buffer,int maxWrite);
unsigned long long  readPipe(int fd,char * buffer,int maxRead);
void writePipeProc(unsigned long long pid, int fd,char * buffer,int maxWrite);
void kernelWrite(char * buffer, int n);
#endif