#include <stdint.h>
#include "Timer.h"
#include "tools.h"
#include "std_io.h"

uint8_t VektorIRQ0, VektorIRQ8;
uint32_t tickCount = 0;
uint32_t sleepCount = 0;
uint8_t sleepFlag = 0;

void SetPIC(char IRQ0, char IRQ8)
{
	VektorIRQ0 = IRQ0;
	VektorIRQ8 = IRQ8;
	//ICW1
	outb(0x20,0x11);
	outb(0xa0,0x11);

	//ICW2
	outb(0x21,VektorIRQ0);
	outb(0xa1,VektorIRQ8);

	//ICW3
	outb(0x21,0x4);
	outb(0xa1,0x2);

	//ICW4
	outb(0x21,0x1);
	outb(0xa1,0x1);

	outb(0x21,0);
	outb(0xa1,0);
}

uint16_t FreqResult = 0;
void SetPIT(uint16_t Freq)
{
	FreqResult = 1193182/Freq; // min 20 hz
	outb(0x43,0x36);

	outb(0x40,FreqResult&0xff);
	outb(0x40,FreqResult>>8);
}

void Timer_initialize(uint16_t freq) {
	SetPIC(0x20,0x28);
	SetPIT(freq);
}

void irqTimer()
{
	asm("pusha");

	tickCount++;
	if (sleepFlag) {
		sleepCount++;
	}
	
	asm("mov al,0x20");
	asm("out 0x20,al");
	asm("popa");
	asm("leave");
	asm("iret");
}

uint32_t getTickCount() {
    return tickCount;
}

void sleep(uint32_t ms) {
	sleepFlag = 1;
	while (sleepCount < ms) {}
	sleepFlag = 1;
	sleepCount = 0;
}