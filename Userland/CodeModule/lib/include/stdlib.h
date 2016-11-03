#ifndef STDLIB_H_
#define STDLIB_H_

#include <stdint.h>

void * malloc(uint64_t size);
void * calloc(uint64_t amount, uint64_t size);
void free(void *);

int atoi(const char * str);

#endif
