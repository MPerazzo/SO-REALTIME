#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include <chunk.h>

typedef struct process_t {
	
	void * heap_start;

	void * stack_start;

	uint64_t heap_size;

	uint64_t stack_size;

	mem_chunk_list_t * heap_node;

} process_t;

#endif
