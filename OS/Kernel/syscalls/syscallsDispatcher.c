#include<syscalls.h>
#include<stdint.h>
#include<video.h>
#include<keyboard.h>
#include<systemInfo.h>
#include<exceptions.h>
#include<files.h>
#include<scheduler.h>
#include<time.h>
//void syscallsDispatcher(uint64_t id, uint64_t rdi, uint64_t rsi, ... ):

//void write(unsigned int fd, const char * buf, uint64_t count);
//void drawScreen(int x,int y, int figure, int scale);

//drawString(int x);


void syscallsDispatcher (uint64_t id, uint64_t * registers){
    switch (id){
        case 0:
            readKeyboard((char *)registers[0], registers[1],(int *)registers[2]);
            break;
        case 1:
            drawCharacter(registers[0],registers[1],registers[2],registers[3]);
            break;
        case 2:
            drawBitmap(registers[0], registers[1],(void *) registers[2]);
            break;
        case 3:
            cpuTemp((int *)registers[0]);
            break;
        case 4:
            readErrors((errorStruct *)registers[0]);
            break;
        case 5:
            setExceptionHandler(registers[0], (void *)registers[1]);
            break;
        case 6:
            scroll(registers[0],registers[1],registers[2],registers[3],registers[4]);
            break;
        case 7:
            retrieveRegs((void *)registers[0],(void *)registers[1]);
            break;
        case 8:
            localtime((void *)registers[0]);
            break;
        case 9:
            drawRect((void *)registers[0]);
            break;
        case 10:
            mapstdout((void *)registers[0],registers[1]);
            break;
        case 11:
            write((void *)registers[0],registers[1]);
            break;
        case 12:
            createProcess((void *)registers[0], registers[1], (char **)registers[2]);
            break;
        case 13:            
            yield();
            break;
        case 14:
            exit(registers[0]);
            break;
        case 15:
            ps((char *)registers[0]);
            break;

        case 16:
            sleep(registers[0]);
            break;
        case 17:
            pipe(registers[0],registers[1],registers[2],registers[3]);
            break;
        case 18:
            readPipe(registers[0],registers[1],registers[2],registers[3]);
            break;
    }

}
