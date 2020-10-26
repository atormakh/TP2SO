#include<syscalls.h>
#include<stdint.h>
#include<video.h>
#include<keyboard.h>
#include<scheduler.h>
#include<time.h>
#include<ipc.h>
#include<pipes.h>
#include<semaphore.h>


unsigned long long syscallsDispatcher (uint64_t id, uint64_t * registers){
    switch (id){
        case 0:
            return readKeyboard((char *)registers[0], registers[1]);
            break;
        case 1:
            drawCharacter(registers[0],registers[1],registers[2],registers[3]);
            break;
        case 2:
            drawBitmap(registers[0], registers[1],(void *) registers[2]);
            break;
        case 3:
            scroll(registers[0],registers[1],registers[2],registers[3],registers[4]);
            break;
        case 4:
            drawRect((void *)registers[0]);
            break;
        case 5:
            return createProcess((void *)registers[0], (char *)registers[1] ,registers[2], (char **)registers[3]);
            break;
        case 6:            
            yield();
            break;
        case 7:
            exit(registers[0]);
            break;
        case 8:
            ps((char *)registers[0]);
            break;

        case 9:
            sleep(registers[0]);
            break;
        case 10:
            pipe((char *)registers[0],registers[1],registers[2],registers[3],registers[4]);
            break;
        case 11:
            return readPipe(registers[0],(void *)registers[1],registers[2]);
            break;
        case 12:
            writePipe(registers[0],(void *)registers[1],registers[2]);
            break;
        case 13:
            openSem((void *)registers[0], registers[1]);
            break;
        case 14:
            semPost((void *)registers[0]);
            break;
        case 15:
            semWait((void *)registers[0]);
            break;
        case 16:
            closeSem((void *)registers[0]);
            break;
        case 17:
            nice(registers[0], registers[1]);
            break;
        case 18:
            return getPid();
            break;
        case 19:
            return kill(registers[0]);
            break;
        case 20:
            return block(registers[0]);
            break;
        case 21:
            return unblock(registers[0]);
            break;
        case 22:
            wait(registers[0]);
            break;
        case 23:
            return (unsigned long long)m_alloc(registers[0]);
            break;
        case 24:
            m_free((void *)registers[0]);
            break;
        case 25:
            return (unsigned long long)c_alloc(registers[0]);
            break;
        case 26:
            pipesInfo((char *)registers[0]);
            break;
        case 27:
            semsInfo((char *)registers[0]);
            break;
        case 28:
            memInfo((char *)registers[0]);
            break;
        case 29:
            closePipe(registers[0]);
            break;
    }
    return id;

}
