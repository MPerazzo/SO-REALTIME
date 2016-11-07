#ifndef __SHARED_H__
#define __SHARED_H__

#include <types.h>
#include <process.h>

#define MAX_SHBLOCKS 10

void init_shared();
int64_t shmget(char *,uint64_t, bool);
void * shmat(uint64_t);
int shmdetach(void *);

typedef struct {
	void * start;
	char * name;
	uint64_t sim_access; /* The amount of processes that opened the mem and could be using it. Works as a counter. */
	
} mem_shared_t;

#endif
