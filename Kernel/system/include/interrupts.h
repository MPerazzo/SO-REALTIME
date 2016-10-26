#ifndef _INTERRUPS_H_
#define _INTERRUPS_H_

#include <types.h>

#define ISR_SIZE 8
#define PIT_IRQ 0
#define KB_IRQ 1

typedef void (*ISR)(void);		// (ISR = Interrupt Service Routine) puntero a funcion de manejo de interrupciones de hardware

void init_interrupts(void);

#endif /* INTERRUPS_H_ */