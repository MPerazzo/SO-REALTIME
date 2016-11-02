#include "buddy.h"
#include "memallocator.h"
#include "types.h"

static uint64_t paged_size(uint64_t size);

uint64_t init_mem(uint64_t size) {
	return buddy_init( paged_size(size) );	
}

void * get_memblock(uint64_t size) {

	int index = buddy_alloc( paged_size(size) );

	if (index == -1)
		return NULL;

	return  (void *)(intptr_t)(MEM_START + index);	
}

static uint64_t paged_size(uint64_t size) {
	
	if (size%PAGE_SIZE == 0)
		return size;

	uint64_t fits;

	fits = size / PAGE_SIZE;

	return PAGE_SIZE*(fits+1);
}