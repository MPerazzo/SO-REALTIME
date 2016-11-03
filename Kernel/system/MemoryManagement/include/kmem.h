#ifndef __KMEM_H__
#define __KMEM_H__

#include <types.h>

#define KH_AVAILABLE_MEMORY 0x300000 // 3 MiB for Kernel RAM heap

#define KH_MEMORY_START 0x600000

#define KH_MEMORY_END KH_MEMORY_START + KH_AVAILABLE_MEMORY - 1

void init_kheap();
void * kmalloc(uint64_t size);
void * kcalloc(uint32_t amount, uint64_t size);
void kfree(void* mem);

#endif
