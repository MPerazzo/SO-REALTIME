#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

#define NULL (void *)0

typedef enum {false = 0, true = 1} bool;

#pragma pack(push)
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
    uint16_t    offset_l;
    uint16_t    selector;
    uint8_t     zero1;
    uint8_t     access;
    uint16_t    offset_m;
    uint32_t    offset_h;
    uint32_t    zero2;
} DESCR_INT;

/* IDTR  */
typedef struct {
    uint16_t  limit;
    uint64_t base;
} IDTR;

#pragma pack(pop)

#endif
