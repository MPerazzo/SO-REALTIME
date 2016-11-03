#include "buddy.h"
#include "memlib.h"
#include "types.h"
#include "kmem.h"
#include "process.h"
#include "pmem.h"

static uint64_t paged_size(uint64_t size);

uint64_t init_mem(uint64_t size) {

	init_kheap();
	return buddy_init( paged_size(size) );	
}

void * get_memblock(uint64_t size) {

	int index = buddy_alloc( paged_size(size) );

	if (index == -1)
		return NULL;

	return (void *)(intptr_t)(MEM_START + index);	
}

uint64_t free_memblock(void * block_pointer) {

	if ((void *)(intptr_t)MEM_START>block_pointer)
		return -1;

	return buddy_free((uint64_t)(block_pointer - MEM_START));
}

uint64_t init_process_heap(process_t * process) {
	pinit_heap(process);
	return 0;
}

void * malloc(process_t * process, uint64_t size) {
	return pmalloc(process, size);
}

void * calloc(process_t * process, uint64_t amount, uint64_t size) {
	return pcalloc(process, amount, size);
}

uint64_t free(process_t * process, void * mem) {
	pfree(process, mem);
	return 0;
}

static uint64_t paged_size(uint64_t size) {
	
	if (size%PAGE_SIZE == 0)
		return size;

	uint64_t fits;

	fits = size / PAGE_SIZE;

	return PAGE_SIZE*(fits+1);
}
