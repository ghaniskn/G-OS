#include "std_io.h"
#include "keyboard.h"
#include "interrupt.h"
#include "Timer.h"

//interrupt gate descriptor
struct IdtEntry{
	unsigned short base_lo;
	unsigned short selector;
	unsigned char reserved;
	unsigned char flags;
	unsigned short base_hi;
} __attribute__ ((packed));

struct IdtRegister{
	unsigned short limit;
	unsigned long  base;
} __attribute__ ((packed));

struct IdtEntry idt[256];
struct IdtRegister idtr;


int IdtSetGate(unsigned int index, void *base, unsigned long selector, unsigned char flags) {
	if(index > 255) return -1;
	idt[index].base_lo = ((long)base) & 0xFFFF;
	idt[index].base_hi = ((long)base) >> 16;
	idt[index].reserved = 0;
	idt[index].selector = selector;
	idt[index].flags = flags;
	return 0;
}

void IDT_initialize(){
	IdtSetGate(0,Div0ExceptionHandler, 8, 0x8e);
	IdtSetGate(0x21,keyboard_irq,8,0x8e);
	IdtSetGate(0x20,irqTimer,8,0x8e);

	idtr.base = (unsigned long)&idt;
	idtr.limit = sizeof(idt);
	asm("lidt idtr");
}
