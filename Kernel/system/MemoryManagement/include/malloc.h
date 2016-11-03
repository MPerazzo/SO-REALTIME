#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <types.h>

#define UH_AVAILABLE_MEMORY 0x300000 // 3 MiB for User RAM heap

#define UH_MEMORY_START 0xA00000

#define UH_MEMORY_END UH_MEMORY_START + UH_AVAILABLE_MEMORY - 1

void init_uheap();
void * malloc(uint64_t size);
void * calloc(uint32_t amount, uint64_t size);
void free(void * mem);

#endif
