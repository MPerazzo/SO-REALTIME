// Source : https://github.com/bear24rw/EECE4029/tree/master/hw5_vmm

#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <stdint.h>
#include <types.h>

/* Node can only be in one of 3 states */
typedef enum {
    FREE = 0,
    SPLIT,
    ALLOC
} node_state;

typedef struct node_t {
    uint64_t idx;                /* index into the pool */
    node_state state;            /* state of the node */
    uint64_t size;               /* how many bytes in the pool */
    struct node_t* left;         /* if split we make two buddies below us */
    struct node_t* right;
} node_t;


uint64_t buddy_init(uint64_t size, bool);
uint64_t buddy_alloc(uint64_t size, bool);
uint64_t buddy_free(uint64_t idx, bool);
uint64_t buddy_size(uint64_t idx, bool);
void buddy_print(bool);
void buddy_kill(bool);

#endif
