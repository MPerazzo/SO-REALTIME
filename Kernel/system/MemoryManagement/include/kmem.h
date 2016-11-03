#ifndef __KMEM_H__
#define __KMEM_H__

#include <types.h>
#include <chunk.h>

#define AVAILABLE_MEMORY 0x300000 // 3 MiB for Kernel RAM heap

#define MEMORY_START 0x600000

#define MEMORY_END MEMORY_START + AVAILABLE_MEMORY - 1

void init_kheap();
void *kmalloc(uint64_t size);
void *kcalloc(uint32_t amount, uint64_t size);
void kfree(void* mem);

#endif
