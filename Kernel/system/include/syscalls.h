#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define STDDATA 3

#define SYSCALLS_SIZE 12

enum {
	SYS_EXIT,
	SYS_CLEAR,
	SYS_READ,
	SYS_WRITE,
	SYS_VIDEO,
	SYS_DRAW,
	SYS_SBRK,
	
	SYS_TIME,
	SYS_DATE,
	SYS_SET_TIME,
	SYS_SET_DATE,
	SYS_WAIT,
};

typedef uint64_t (*SYSCALL)(uint64_t arg1, uint64_t arg2, uint64_t arg3);	// puntero a funcion de manejo de una syscall

void init_syscalls(void);

#endif