
#include "pmem.h"
#include "lib.h"
#include "process.h"
#include "types.h"

static inline int64_t abs(int64_t n);

static inline int64_t abs(int64_t n) {
  return n > 0 ? n : -n;
}

void * pmalloc(process_t * process, uint64_t size) {

	mem_chunk_t *chunk;
	mem_chunk_list_t *curr, *prev;

	curr = process->heap_node;
	prev = NULL;

	while (curr != NULL && (curr->chunk->used || curr->chunk->size < size)) {
		prev = curr;
		curr = curr->next;
	}

	if (curr) {

		curr->chunk->size = size;
		curr->chunk->used = true;

		return curr->chunk->start;
	}

	if ( (uint64_t)prev->chunk->start + prev->chunk->size + size > ((uint64_t)process->heap_start + process->heap_size) ) {
		return NULL;
	}

	chunk = (mem_chunk_t*)(prev->chunk->start + prev->chunk->size);

	memset(chunk, 0, sizeof(mem_chunk_t));
	chunk->start = (void*)(((uint64_t)chunk) + sizeof(mem_chunk_t) + sizeof(mem_chunk_list_t));
	chunk->size = size;
	chunk->used = true;

	curr = (mem_chunk_list_t*)( ((uint64_t)chunk) + sizeof(mem_chunk_t));
	memset(curr, 0, sizeof(mem_chunk_list_t));
	curr->chunk = chunk;
	curr->next = NULL;

	prev->next = curr;

	return chunk->start;
}

void * pcalloc(process_t * process, uint32_t amount, uint64_t size) {

	void *mem;
	if ( (mem = pmalloc(process, size*amount))  ) {
		memset(mem, 0, size*amount);
	}
	return mem;
}

void pfree(process_t * process, void * mem) {

	mem_chunk_list_t *curr, *prev;
	bool append_to_prev = false;

	prev = NULL;
	curr = process->heap_node;

	while (curr != NULL && abs(mem - curr->chunk->start) > curr->chunk->size) {
		prev = curr;
		curr = curr->next;
	}

	if (curr != NULL) {
		curr->chunk->used = false;
		if (prev && !prev->chunk->used) {
			prev->chunk->size += curr->chunk->size;
			prev->next = curr->next;
			append_to_prev = true;
		}
		if (curr->next && !curr->next->chunk->used) {
			if (append_to_prev) {
				prev->chunk->size += curr->next->chunk->size;
				prev->next = curr->next->next;
			} else {
				curr->chunk->size += curr->next->chunk->size;
				curr->next = curr->next->next;
			}
		}
	}
}

void pinit_heap(process_t * process) {

	mem_chunk_t *chunk;
	uint16_t size;

	size = sizeof(mem_chunk_t) + sizeof(mem_chunk_list_t);

	chunk = (mem_chunk_t*)(process->heap_start);
	memset(chunk, 0, size);

	chunk->start = (void*)chunk;
	chunk->used = true;
	chunk->size = size;

	process->heap_node = (void*)(process->heap_start + sizeof(mem_chunk_t));
	process->heap_node->next = NULL;
	process->heap_node->chunk = chunk;

}
