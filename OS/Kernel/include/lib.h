#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void hlt();
void putchar(char);
int strlen(char * string);
void printf(char * fmt, ...);
void puts(char * buffer);
void deleteChar(char * str, char c);
int intToHex(unsigned long long num, char * buffer);
int intToBase(unsigned long long num, int base, char*buffer);
int intToString(unsigned long long num, char * buffer);
int strcpy(char * dest, char * source);
int strcmp(char * str1, char * str2);
int doubleToString(double value, char * buffer);
double stringToDouble(char *init, char *end);
unsigned long strHash(char *str);
int stringToInt(char * buffer);




void setBit(unsigned char * byte, int bitNumber);
void clearBit(unsigned char * byte, int bitNumber);
char getBit(unsigned char * byte, int bitNumber);
unsigned int Log2n(unsigned int n);
unsigned long long pow(unsigned int base, unsigned int exp);



#endif