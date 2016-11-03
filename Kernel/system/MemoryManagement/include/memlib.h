#ifndef __MEMLIB_H__
#define __MEMLIB_H__

#include <process.h>

#define PAGE_SIZE 4096
#define MEM_START 0x600000

uint64_t init_mem(uint64_t);
void * get_memblock(uint64_t);
uint64_t free_memblock( void *);
uint64_t init_process_heap(process_t *);
void * malloc(process_t *, uint64_t);
void * calloc(process_t *, uint64_t, uint64_t);
uint64_t free(process_t *, void *);

#endif
