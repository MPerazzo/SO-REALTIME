#include <stdlib.h>
#include <syscalls.h>

void * malloc(uint64_t size) {
	return s_malloc(size);
}

void * calloc(uint64_t amount, uint64_t size) {
	return s_calloc(amount, size);
}

void free(void * ptr) {
	return s_free(ptr);
}

int atoi(const char * str) {
	char c;
	int i = 0;
	while ((c = *str++) != 0) {
		if (c < '0' || c > '9') {
			break;
		}
		i = i * 10 + c - '0';
	}
	return i;
}
