#ifndef STDIO_H_
#define STDIO_H_

#include <stdint.h>

#define NULL 0

void printNum(uint64_t num);
void hlt();
void putchar(char);
int strlen(char * string);
void printf(char * fmt, ...);
void puts(char * buffer);
int getChar();
uint64_t atoi(char *);
void deleteChar(char * str, char c);
int intToHex(unsigned long long num, char * buffer);
int intToBase(unsigned long long num, int base, char*buffer);
int intToString(unsigned long long num, char * buffer);
int strcpy(char * dest, char * source);
int strcmp(char * str1, char * str2);
int doubleToString(double value, char * buffer);
double stringToDouble(char *init, char *end);
int stringToInt(char * buffer);
void * stringToPtr(char * buffer);



#endif