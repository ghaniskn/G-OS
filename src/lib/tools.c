#include "tools.h"

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void outb(unsigned short port, unsigned char data) {
	asm("mov dx,%0"::"g"(port));
	asm("mov al,%0"::"g"(data));
	asm("out dx,al");
}

unsigned char inb(unsigned short port) {
	asm("mov dx,%0"::"g"(port));
	asm("in al,dx");
}

void outw(unsigned short port, unsigned short data) {
	asm("mov dx,%0"::"g"(port));
	asm("mov ax,%0"::"g"(data));
	asm("out dx,ax");
}

unsigned short inw(unsigned short port) {
	asm("mov dx,%0"::"g"(port));
	asm("in ax,dx");
}

void outl(unsigned short port, unsigned long data) {
	asm("mov dx,%0"::"g"(port));
	asm("mov eax,%0"::"g"(data));
	asm("out dx,eax");
}

unsigned long inl(unsigned short port) {
	asm("mov dx,%0"::"g"(port));
	asm("in eax,dx");
}

int strcmp (const char *p1, const char *p2) {
  register const unsigned char *s1 = (const unsigned char *) p1;
  register const unsigned char *s2 = (const unsigned char *) p2;
  register unsigned char c1, c2;

  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);

  return c1 - c2;
}
