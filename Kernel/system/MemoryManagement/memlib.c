#include "buddy.h"
#include "memlib.h"
#include "types.h"
#include "kmem.h"
#include "malloc.h"

static uint64_t paged_size(uint64_t size);

uint64_t init_mem(uint64_t size, bool kernelStructures) {
	return buddy_init( paged_size(size), kernelStructures );	
}

void * get_memblock(uint64_t size, bool kernelStructures) {

	int index = buddy_alloc( paged_size(size), kernelStructures );

	uint64_t mem_start;

	if (index == -1)
		return NULL;

	if (kernelStructures)
		mem_start = KH_MEMORY_START;
	else
		mem_start = UH_MEMORY_START;

	return  (void *)(intptr_t)(mem_start + index);	
}

uint64_t free_memblock( void * block_pointer, bool kernelStructures) {
	
	uint64_t mem_start;

	if (kernelStructures)
		mem_start = KH_MEMORY_START;
	else
		mem_start = UH_MEMORY_START;

	if ((void *)(intptr_t)mem_start>block_pointer)
		return -1;

	return buddy_free( (uint64_t) (block_pointer - mem_start), kernelStructures);
}

static uint64_t paged_size(uint64_t size) {
	
	if (size%PAGE_SIZE == 0)
		return size;

	uint64_t fits;

	fits = size / PAGE_SIZE;

	return PAGE_SIZE*(fits+1);
}
