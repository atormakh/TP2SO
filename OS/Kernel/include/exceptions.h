#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdint.h>
#include <registersArgs.h>

typedef struct errorStruct{
	char errorCode;
	uint64_t opcode;
	registerArgs registers;
}errorStruct;




void readErrors(errorStruct * error);

void setExceptionHandler(int ex, void * handler);



#endif