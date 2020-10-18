#ifndef PIPES_H
#define PIPES_H

#define WRITE 2
#define READ 1
int pipe(unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead );
void closePipe(int fd);
void closePipeProc(int fd, unsigned long long pid);

typedef struct Pipe{
    unsigned int writeIndex;
    unsigned char status;
    unsigned int readIndex;
    unsigned int bufferSize;
    unsigned int counter;
    unsigned int writerRefs;
    unsigned int readerRefs;
    unsigned char * buffer;
} Pipe;

#endif