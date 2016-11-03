#ifndef __MEMLIB_H__
#define __MEMLIB_H__

#define PAGE_SIZE 4096
#define MEM_START 0x600000

uint64_t init_mem(uint64_t, bool);
void * get_memblock(uint64_t, bool);
uint64_t free_memblock( void *, bool);

#endif
