#include <syscalls.h>

extern uint64_t _syscall(uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t arg3);

int exit(int code) {
	return _syscall(SYS_EXIT, code, 0, 0);
}

void clear(void) {
	_syscall(SYS_CLEAR, 0, 0, 0);
}

int read(unsigned int fd, const void * buffer, unsigned int size) {
	return _syscall(SYS_READ, fd, (unsigned long)buffer, size);
}

int write(unsigned int fd, const void * buffer, unsigned int size) {
	return _syscall(SYS_WRITE, fd, (unsigned long)buffer, size);
}

int video(unsigned int width, unsigned int height, unsigned int bpp) {
	return _syscall(SYS_VIDEO, width, height, bpp);
}

int draw(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b) {
	unsigned int color = 0;
	color += r;
	color = color << 8;
	color += g;
	color = color << 8;
	color += b;
	return _syscall(SYS_DRAW, x, y, color);
}

void * sbrk(unsigned int increment) {
	return (void *)_syscall(SYS_SBRK, increment, 0, 0);
}

void * memblock(unsigned int size) {
	return (void *)_syscall(SYS_MEMBLOCK, size, 0, 0);
}

int freeblock(void * block_start) {
	return _syscall(SYS_FREEBLOCK, (unsigned long)block_start, 0, 0);
}

void * s_malloc(unsigned int size) {
	return (void *)_syscall(SYS_MALLOC, size, 0, 0);
}

void * s_calloc(unsigned int amount, unsigned int size) {
	return (void *)_syscall(SYS_CALLOC, amount, size, 0);
}

void s_free(void * mem) {
	_syscall(SYS_FREE, (uint64_t)mem, 0, 0);
}

void time(void) {
	_syscall(SYS_TIME, 0, 0, 0);
}

char * date(void) {
	return (char *)_syscall(SYS_DATE, 0, 0, 0);
}

int set_time(unsigned int hour, unsigned int minutes, unsigned int seconds) {
	return _syscall(SYS_SET_TIME, hour, minutes, seconds);
}

int set_date(unsigned int day, unsigned int month, unsigned int year) {
	return _syscall(SYS_SET_DATE, day, month, year);
}

int wait(unsigned long millis) {
	return _syscall(SYS_WAIT, millis, 0, 0);
}
