#include <shared.h>
#include <types.h>
#include <memlib.h>
#include <kmem.h>
#include <util.h>

bool freeSpace();

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

	bool equals = false;

	if (name==NULL || size<0)
		return -1;

	uint64_t pos = -1;
	uint64_t j;

	for (j=0; j<current; j++) {

		if (mems[j] == NULL)
			pos=j;

		else {

			if (strcmp(mems[j]->name, name) == 0) {
				equals = true;
				pos=j;
				break;
			}
		}
	}

	if (pos == -1) // no space avaible
		return -1;

	uint64_t id = pos + 1;

	if (overwrite || (!overwrite && !equals) ) {

		void * aux = kmalloc(sizeof(mem_shared_t));

		if (aux == NULL)
			return -1;

		mems[pos] = (mem_shared_t *)aux;

		mems[pos]->name = kmalloc(strlen(name) +1);

		strcpy(mems[pos]->name, name);

		mems[pos]->start = get_memblock(size);

		mems[pos]->sim_access = 0;
	}

	if (!equals && !freeSpace())
		current++;

	return id;
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

bool freeSpace() {

	int j;

	for (j=0; j<current; j++) {

		if (mems[j] == NULL)
			return true;
	}

	return false;
}

