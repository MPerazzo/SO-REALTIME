#ifndef __PMEM_H__
#define __PMEM_H__

#include <process.h>
#include <chunk.h>

void * pmalloc(process_t * process, uint64_t size);
void * pcalloc(process_t * process, uint32_t amount, uint64_t size);
void pfree(process_t * process, void * mem);
void pinit_heap(process_t * process);

#endif
