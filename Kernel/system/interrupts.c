#include <types.h>
#include <keyboard.h>
#include <timer.h>
#include <video.h>
#include <stdint.h>
#include <interrupts.h>

static void set_idt_entry (int index, uint8_t selector, uint64_t offset, uint8_t access);

extern void _irq00Handler(void);
extern void _irq01Handler(void);
extern void _irq02Handler(void);
extern void _irq03Handler(void);
extern void _irq04Handler(void);
extern void _irq05Handler(void);
extern void _irq80Handler(void);

extern void _sti();
extern void _cli();
extern void picMasterMask(uint8_t mask);
extern void picSlaveMask(uint8_t mask);

DESCR_INT * idt;
static ISR routines[ISR_SIZE];				// array de punteros a funcion para las interrupciones

/* Sets all idt entries, uses the idtr provided by Pure64 */
static void init_idt() {
    idt = 0;

    set_idt_entry(0x20, 0x08, (uint64_t)&_irq00Handler, 0x8E);
    set_idt_entry(0x21, 0x08, (uint64_t)&_irq01Handler, 0x8E);    
    set_idt_entry(0x80, 0x08, (uint64_t)&_irq80Handler, 0x8E);  

    picMasterMask(0xFC); 
    picSlaveMask(0xFF);
}

static void set_idt_entry (int index, uint8_t selector, uint64_t offset, uint8_t access) {
    idt[index].selector = selector;
    idt[index].offset_l = offset & 0xFFFF;
    idt[index].offset_m = (offset >> 16) & 0xFFFF;
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
    idt[index].access = access;
    idt[index].zero1 = 0;
    idt[index].zero2 = 0;
}

void irqDispatcher(uint32_t irq) {
	routines[irq]();
}

/* Initializes hardware interrupts */
void init_interrupts() {
	/* Initilaze idt */
	init_idt();

	/* Initialize ISRs */
	routines[PIT_IRQ] = timer_handler;
	routines[KB_IRQ] = keyboard_handler;

	/**/
	_sti();
}