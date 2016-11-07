#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include <chunk.h>

#define SIGKILL 0
#define SIGSTOP 1
#define SIGCONT 2

typedef enum {

	RUNNING = 0,
	BLOCKED,
	READY,
	ENDED

} ProcessState;

typedef struct process_t {
	
	void * heap_start;

	void * stack_start;

	uint64_t heap_size;

	uint64_t stack_size;

	mem_chunk_list_t * heap_node;

	char * name;

	pid_t pid;

	ProcessState processState;

	sig_t signalHandler;

	void * userStack;
	void * kernelStack;
	void * entryPoint;

} process_t;

typedef uint64_t pid_t;

typedef void (*sig_t)(int);	// pointer to signal handler

#endif
