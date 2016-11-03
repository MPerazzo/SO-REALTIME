#ifndef __MEMCHUNK_H__
#define __MEMCHUNK_H__

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
