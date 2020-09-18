
#include <stdint.h>
#include <exceptions.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6
#define ERROR_BUFFER_SIZE 10
#define EXCEPTIONS_LENGTH 7

static void zero_division(registerArgs * args);
static void invalid_opcode(registerArgs * args);

static void(*exceptionsHandlers[EXCEPTIONS_LENGTH])(errorStruct *);

static errorStruct buffer[ERROR_BUFFER_SIZE];
static int errorIndex = -1;


//exception es el codigo que le corresponde al error
//args es una serie de parametros. args = {r15,...,r8,rsi,rdi,rbp,rdx,rcx,rbx,rax,rip,cs,flags,rsp,ss}
void exceptionDispatcher(int exception,registerArgs * args) {
	switch(exception){
		case ZERO_EXCEPTION_ID:
			zero_division(args);
			break;
		case INVALID_OPCODE_EXCEPTION_ID:
			invalid_opcode(args);
			break;
	}
}

//char * errorMessages[6] = {"Problema 1"," 2","3","4","5",};

static void zero_division(registerArgs * args) {
	// Handler para zero divisions
	errorStruct * error = buffer + (++errorIndex%ERROR_BUFFER_SIZE);
	error -> errorCode = ZERO_EXCEPTION_ID;
	error-> opcode = *((char *)args -> rip);
	error -> registers = *args; 
	
	exceptionsHandlers[0](error);

}

static void invalid_opcode(registerArgs * args){
	//Handler para manejar invalid 
	errorStruct * error = buffer + (++errorIndex%ERROR_BUFFER_SIZE);
	error -> errorCode = INVALID_OPCODE_EXCEPTION_ID;
	error-> opcode = *((char *)args -> rip);
	error -> registers = *args;
	exceptionsHandlers[6](error); 
}

void readErrors(errorStruct * error){
	if(errorIndex < 0){
		error->errorCode = -1;
		return;
	}
	*error = buffer[errorIndex--];
}

void setExceptionHandler(int ex, void * handler){
	exceptionsHandlers[ex] = handler;

}