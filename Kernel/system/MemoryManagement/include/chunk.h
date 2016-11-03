#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <types.h>

typedef struct {
	void *start;
	uint64_t size;
	bool used;
} mem_chunk_t;

typedef struct mem_chunk_list_t {
	mem_chunk_t* chunk;
	struct mem_chunk_list_t* next;
} mem_chunk_list_t;

#endif
