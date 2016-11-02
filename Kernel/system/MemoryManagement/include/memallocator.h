#define PAGE_SIZE 4096
#define MEM_START 0x600000

uint64_t init_mem(uint64_t size);
void * get_memblock(uint64_t size);
