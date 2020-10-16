#include <stdint.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}


int intToBase(unsigned long long num, int base, char*buffer){
    char stack[11];
    int c = 0;
    int i=0;
    int remainder = 0;
    if(num==0) stack[i++]='0';
    while(num!=0){
        remainder = num % base;
        stack[i]=remainder>=10? remainder+'A'-10:remainder+'0';
        num = num/base;
        i++;
    }
    c=i;
    i--;
    while(i>=0){
        *buffer=stack[i];
        buffer++;
        i--;
    }
    *buffer=0;
    return c;
}


int intToString(unsigned long long num, char * buffer){
    return intToBase(num,10,buffer);
}


int strcmp(char * str1, char * str2){
    int i =0;
    while(str1[i] != 0 && str2[i] != 0 ){
        if(str1[i] != str2[i]){
            return str1[i]-str2[i];
        }
        i++;
    }
    return str1[i]-str2[i];
}

int strcpy(char * dest, char * src){
    unsigned char i =0;
    while(src[i]!=0){
        dest[i] = src[i];
        i++;
    }
    dest[i]=0;
    return i;

}


void setBit(unsigned char * byte, int bitNumber){
    unsigned char mask = 1;
    mask=mask << bitNumber;
    *byte=*byte | mask;
}
void clearBit(unsigned char * byte, int bitNumber){
    char mask = 0;
    mask=mask << bitNumber;
    *byte=*byte & mask;
}
char getBit(unsigned char * byte, int bitNumber){
    unsigned char ret = *byte;
    ret=ret>>bitNumber;
    return ret%2;
}

unsigned int Log2n(unsigned int n) { 
    return (n > 1) ? 1 + Log2n(n >> 1) : 0; 
}

unsigned long long pow(unsigned int base, unsigned int exp){
    unsigned long long ret=1;
    for(int i=0;i<exp;i++) ret*=base;
    return ret;
}

