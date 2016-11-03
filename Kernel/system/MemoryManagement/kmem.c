
#include "kmem.h"
#include "lib.h"
#include "memchunk.h"

static inline int64_t abs(int64_t n);

static mem_chunk_list_t* allocated = NULL;

static inline int64_t abs(int64_t n) {
  return n > 0 ? n : -n;
}

void * kmalloc(uint64_t size) {

	mem_chunk_t *chunk;
	mem_chunk_list_t * curr, *prev;

	curr = allocated;
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

	if ( (uint64_t)prev->chunk->start + size > KH_MEMORY_END ) {
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

void * kcalloc(uint32_t amount, uint64_t size) {

	void *mem;
	if ( (mem = kmalloc(size*amount))  ) {
		memset(mem, 0, size*amount);
	}
	return mem;
}

void kfree(void * mem) {

	mem_chunk_list_t* curr, *prev;
	bool append_to_prev = false;

	prev = NULL;
	curr = allocated;

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

void init_kheap() {

	mem_chunk_t *chunk;
	uint16_t size;

	size = sizeof(mem_chunk_t) + sizeof(mem_chunk_list_t);

	chunk = (mem_chunk_t*)(long int)(KH_MEMORY_START);
	memset(chunk, 0, size);

	chunk->start = (void*)chunk;
	chunk->used = true;
	chunk->size = size;

	allocated = (void*)(KH_MEMORY_START + sizeof(mem_chunk_t));
	allocated->next = NULL;
	allocated->chunk = chunk;

}
