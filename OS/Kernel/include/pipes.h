#ifndef PIPES_H
#define PIPES_H

#define WRITE 2
#define READ 1
int pipe(unsigned long pidWriter, unsigned int fdWrite, unsigned long pidReader, unsigned int fdRead );

typedef struct Pipe{
    unsigned int writeIndex;
    unsigned char status;
    unsigned int readIndex;
    unsigned int bufferSize;
    unsigned int counter;
    unsigned char * buffer;
} Pipe;

#endif