#include "std_io.h"
#include "tools.h"

size_t io_cursor_y;
size_t io_cursor_x;
uint8_t io_color;
uint16_t* io_buffer = (uint16_t*) 0xB8000;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void vga_cursor_update(unsigned short x, unsigned short y)
{
	unsigned short position = (y * VGA_WIDTH) + x;
	
	outb(0x3d4,14);
	outb(0x3d5,position>>8);
	outb(0x3d4,15);
	outb(0x3d5,position & 0xff);
}
 
void io_setcolor(uint8_t color) 
{
	io_color = color;
}
 
void io_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	io_buffer[index] = vga_entry(c, color);
}

void io_setcursor(size_t x, size_t y) 
{
	io_cursor_x = x;
	io_cursor_y = y;
	vga_cursor_update(io_cursor_x,io_cursor_y);
}

void io_newline()
{
	unsigned int i=0;
	unsigned char* content=(unsigned char*)0xb8000;
	
	if (io_cursor_y == 24)
	{
		for(i=0; i<(VGA_WIDTH*(VGA_HEIGHT-1)); i++)
		{
			io_buffer[i] = io_buffer[i + VGA_WIDTH];
		}

		for(i=(VGA_WIDTH*(VGA_HEIGHT-1)); i<(VGA_WIDTH*VGA_HEIGHT); i++)
		{
			io_buffer[i] = (uint16_t) 0 | (uint16_t) io_color << 8;
		}

		io_cursor_y = 24;
		io_cursor_x = 0;
	}
	else
	{
		io_cursor_y++;
		io_cursor_x=0;
	}
	vga_cursor_update(io_cursor_x,io_cursor_y);
}
 
void io_putchar(char c) 
{
	if (c == '\n') io_newline();

	else if (c == 8) {
		size_t index = io_cursor_y * VGA_WIDTH + io_cursor_x;
		index--;
		io_buffer[index] = vga_entry(0, io_color);

		if(io_cursor_x != 0) {
			io_cursor_x--;
		} else {
			if (io_cursor_y == 0) return;

			io_cursor_x = 79;
			io_cursor_y--;
		}
		vga_cursor_update(io_cursor_x, io_cursor_y);
	}

	else {
		io_putentryat(c, io_color, io_cursor_x, io_cursor_y);
		if (++io_cursor_x == VGA_WIDTH) {
			io_cursor_x = 0;
			if (++io_cursor_y == VGA_HEIGHT)
				io_cursor_y = 0;
		}
		vga_cursor_update(io_cursor_x, io_cursor_y);
	}
}
 
void io_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		io_putchar(data[i]);
}
 
void io_print(const char* data) 
{
	io_write(data, strlen(data));
}

void io_clear()
{
	io_setcursor(0, 0);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			io_buffer[index] = vga_entry(' ', io_color);
		}
	}
}