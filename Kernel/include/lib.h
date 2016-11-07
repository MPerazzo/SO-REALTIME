#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void * memsetw(void * destination, unsigned short val, int count);
int memcmp(const void* s1, const void* s2, uint64_t n);

char *cpuVendor(char *result);

#endif
