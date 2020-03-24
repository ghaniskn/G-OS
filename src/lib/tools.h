#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short data);
unsigned short inw(unsigned short port);
void outl(unsigned short port, unsigned long data);
unsigned long inl(unsigned short port);

int strcmp (const char *p1, const char *p2);