#include "buddy.h"
#include "kmem.h"
#include "malloc.h"
#include "video.h"
#include "types.h"
/* TODO: 
   *New library for buddy print?
   *Heap for processes.
   *Test user and kernel trees.
*/

uint64_t k_buddy_init(uint64_t);
uint64_t u_buddy_init(uint64_t);


void * k_buddy_pool;
void * u_buddy_pool;

static node_t * k_buddy_head;
static node_t * u_buddy_head;

void *(*allocatemem)(uint64_t);

void (*freemem)(void *);

/*
 * Attempt to allocate of pool of size 'size'
 * and intialize the head node. Returns -1
 * if either pool or head node could not be
 * allocated.
 */
uint64_t buddy_init(uint64_t size, bool kernelStructures) {

    if (kernelStructures)
        return k_buddy_init(size);

    return u_buddy_init(size);
}

uint64_t k_buddy_init(uint64_t size) {

    /* allocate space for all the pages */
    k_buddy_pool = kmalloc(size);

    if (!k_buddy_pool) {
        return -1;
    }

    /* allocate the head node of the tree */
    k_buddy_head = (node_t*)kmalloc(sizeof(node_t));

    if (!k_buddy_head) {
        return -1;
    }

    k_buddy_head->left = NULL;
    k_buddy_head->right = NULL;
    k_buddy_head->state = FREE;
    k_buddy_head->size = size;
    k_buddy_head->idx = 0;

    return 0;
}

uint64_t u_buddy_init(uint64_t size) {

    /* allocate space for all the pages */
    u_buddy_pool = malloc(size);

    if (!u_buddy_pool) {
        return -1;
    }

    /* allocate the head node of the tree */
    u_buddy_head = (node_t*)malloc(sizeof(node_t));

    if (!u_buddy_head) {
        return -1;
    }

    u_buddy_head->left = NULL;
    u_buddy_head->right = NULL;
    u_buddy_head->state = FREE;
    u_buddy_head->size = size;
    u_buddy_head->idx = 0;

    return 0;
}


/*
 * Attempt to allocate a page of size 'size'
 * returns page index on success or -1 if the
 * requested size could not fit.
 */
uint64_t _buddy_alloc(node_t *n, uint64_t size) {

    int64_t rt = -1;

    /* if this node is a split then recurse down the
     * left side. If the left side is not successful
     * then try the right side.
     */
    if (n->state == SPLIT) {

        rt = _buddy_alloc(n->left, size);
        if (rt >= 0) {
            return rt;
        }
        return _buddy_alloc(n->right, size);
    }

    /* if were not free return error */
    if (n->state != FREE) {
        return -1;
    }

    /* if were too small return error */
    if (n->size < size) {
        return -1;
    }

    /* check if we have enough space to make a split */
    if ( (n->size >> 1) >= size) {

        /* we do, mark this node as SPLIT */
        n->state = SPLIT;

        /* make two new nodes below us with half our size */
        n->left = (node_t*)allocatemem(sizeof(node_t));
        n->right = (node_t*)allocatemem(sizeof(node_t));
        
        /* mark them as free */
        n->left->state = FREE;
        n->right->state = FREE;
        
        /* set their sizes to half */
        n->left->size = n->size >> 1;
        n->right->size = n->size >> 1;
       
        /* set their indexes */
        n->left->idx = n->idx;
        n->right->idx = (n->idx + (n->size >> 1));
       
        /* set their children */
        n->left->left = NULL;
        n->right->right = NULL;

        /* keep traversing down the tree */
        return _buddy_alloc(n->left, size);

    }

    /* if we've gotten here we are free but not big enough to split */
    n->state = ALLOC;
    return n->idx;
}

/*
 * Attempt to mark page 'idx' as FREE
 * Also merge two pages together if they
 * are buddies and both free. Returns 0
 * if 'idx' was marked FREE or -1 if 'idx'
 * was not found.
 */
uint64_t _buddy_free(node_t *n, uint64_t idx) {
    uint64_t rt_left = -1;
    uint64_t rt_right = -1;

    /* if this node is split we want to recurse down each path */
    if (n->state == SPLIT) {

        /* recurse down each path */
        rt_left = _buddy_free(n->left, idx);
        rt_right = _buddy_free(n->right, idx);

        /* check if we should merge this split */
        if (n->left->state == FREE && n->right->state == FREE) {
            freemem(n->left);
            freemem(n->right);
            n->left = NULL;
            n->right = NULL;
            n->state = FREE;
        }

        /* if either path freed the page return success */
        if (rt_left == 0 || rt_right == 0) return 0;

        /* neither path freed the page */
        return -1;
    }


    /* if this nodes index matches the one were searching
     * for and it is allocated mark it as FREE.
     */
    if (n->idx == idx && n->state == ALLOC) {
        n->state = FREE;
        return 0;
    }

    /* this is not the node we are looking for */
    return -1;
}

/*
 * Attempts to return the number of bytes until
 * the end of the page containing 'idx'. Ex. if
 * page starts at idx 0 and is 10 bytes long passing
 * 7 to this function will return 3.
 */
uint64_t _buddy_size(node_t *n, uint64_t idx) {
    
    uint64_t rt = -1;

    /* if this node is a split then recurse down the
     * right side first. If the right side is not successful
     * then try the left side. Right side first because will
     * have higher idx values first.
     */
    if (n->state == SPLIT) {
        rt = _buddy_size(n->right, idx);
        if (rt < 0)
            return _buddy_size(n->left, idx);
        else
            return rt;
    }

    /* if the index we're searching for is to the right
     * of this node and to the left of the end of it and
     * this node is allocated return the distance to the end.
     */
    if ((idx >= n->idx) &&
        (idx < n->idx + n->size) &&
        (n->state == ALLOC))
    {
        return (n->idx + n->size) - idx;
    }

    /* this is not the node we are looking for */
    return -1;
}

/*
 * Walks down the whole tree and frees every node.
 */
void _buddy_kill(node_t *n) {

    /* if this node is split we want to recurse down each path */
    if (n->state == SPLIT) {
        _buddy_kill(n->left);
        _buddy_kill(n->right);
    }

    /* free ourself */
    freemem(n);
}

/*
 * Walk the tree and draw all allocated
 * and free space. If the space is allocated
 * it's index value is displayed. If space is
 * free a dash (-) is displayed.
 */
void _buddy_print(node_t *n) {

    uint64_t i;

    /* if this node is split we want to recurse down each path */
    if (n->state == SPLIT) {
        _buddy_print(n->left);
        _buddy_print(n->right);
        return;
    }

    //this node is either ALLOC or FREE so draw it 
    for (i = 0; i < n->size; i++) {
        if (n->state == FREE) {
            puts("-,", LIGHT_GREY);
        } else {
            putnumber(n->idx, LIGHT_GREY);
            puts(",", LIGHT_GREY);
        }
    }
    puts("|", LIGHT_GREY);
}

/* recursion wrappers */

uint64_t buddy_alloc(uint64_t size, bool kernelStructures) {

    if (kernelStructures) {

        allocatemem = &kmalloc;

        return _buddy_alloc(k_buddy_head, size);
    }

    allocatemem = &malloc;

    return _buddy_alloc(u_buddy_head, size);
}

uint64_t buddy_free(uint64_t idx, bool kernelStructures) { 
    
    if (kernelStructures) {

        freemem = &kfree;

        return _buddy_free(k_buddy_head, idx);
    }

    freemem = &free;

    return _buddy_free(u_buddy_head, idx);
}

uint64_t buddy_size(uint64_t idx, bool kernelStructures) { 
    
    if (kernelStructures)
        return _buddy_free(k_buddy_head, idx);

    return _buddy_free(u_buddy_head, idx);
}

void buddy_print(bool kernelStructures) {
	
    puts("Buddy print is:\n", LIGHT_GREY);
	
    if (kernelStructures)
        _buddy_print(k_buddy_head);

    else
        _buddy_print(u_buddy_head);

	puts(" --END--\n", LIGHT_GREY); 
}

void buddy_kill(bool kernelStructures) { 
    
    if (kernelStructures) {

        freemem = &kfree;

        _buddy_kill(k_buddy_head);
    }

    else {

        freemem = &free;

        _buddy_kill(u_buddy_head);

    }
}

