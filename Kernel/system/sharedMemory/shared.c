#include <shared.h>
#include <types.h>
#include <memlib.h>
#include <kmem.h>
#include <util.h>

uint64_t current;

mem_shared_t * mems[MAX_SHBLOCKS];

void init_shared() {

	current = 1;

	int i;
	for (i=0; i<MAX_SHBLOCKS; i++)
		mems[i]=NULL;
}


int64_t shmget(char * name, uint64_t size, bool overwrite) {

	if (current == MAX_SHBLOCKS)
		return -1;

	bool founded = false;

	if (name==NULL || size<0)
		return -1;

	int id = -1;
	int j;

	for (j=0; j<current; j++) {

		if (mems[j] == NULL)
			id=j;

		else {

			if (strcmp(mems[j]->name, name) == 0) {
				if (overwrite) {
					founded = true;
					id=j;
					break;
				}
				else
					return j+1;
			}
		}
	}

	if (id == -1) // no space avaible
		return -1;

	void * aux = kmalloc(sizeof(mem_shared_t));

	if (aux == NULL)
		return -1;

	mems[id] = (mem_shared_t *)aux;

	mems[id]->name = kmalloc(strlen(name) +1);

	strcpy(mems[id]->name, name);

	mems[id]->start = get_memblock(size);

	mems[id]->sim_access = 0;

	if (!founded)
		current++;

	return id+1;
}

void * shmat(uint64_t id) {

	if (id<=0)
		return NULL;

	id = id - 1;

	if (id>=current || mems[id]==NULL)
		return NULL;

	mems[id]->sim_access++;

	return mems[id]->start;
}

int shmdetach(void * address) {

	int j;

	for (j=0; j<current; j++) {

		if (mems[j] != NULL) {

			if (mems[j]->start == address) {

				mems[j]->sim_access--;

				if (mems[j]->sim_access == 0) {

					free_memblock(mems[j]->start);

					kfree(mems[j]->name);

					kfree(mems[j]);

					mems[j] = NULL;

					return 0;
				}
			}
		}
	}

	return -1;
}
