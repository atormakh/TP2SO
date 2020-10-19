#ifndef PIPES_H
#define PIPES_H

#define WRITE 2
#define READ 1
#define MAX_PIPE_NAME 100
#define UNNAMED "unnamed"
int pipe(char * pipeId,unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead );
void closePipe(int fd);
void closePipeProc(int fd, unsigned long long pid);
void initPipes();
void pipesInfo(char *buffer);

typedef struct Pipe{
    unsigned int writeIndex;
    unsigned char status;
    unsigned int readIndex;
    unsigned int bufferSize;
    unsigned int counter;
    unsigned int writerRefs;
    unsigned int readerRefs;
    char * pipeId;
    unsigned char * buffer;
} Pipe;

#endif