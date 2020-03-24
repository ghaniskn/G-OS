#include <stdint.h>

uint32_t* video_framebuffer;
uint32_t background_color;

void vbe_set(uint16_t xres, uint16_t yres, uint16_t bpp);
uint32_t get_colorCode(uint8_t red, uint8_t green, uint8_t blue);

void disp_setBGColor(uint32_t colorcode);
void disp_putPixel(uint16_t x, uint16_t y, uint32_t colorcode);
void disp_clear(uint32_t colorcode);
void disp_check();
void disp_drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t colorcode);
void disp_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t colorcode);
void disp_drawChar(uint8_t c, uint16_t x, uint16_t y, uint32_t fg_color, uint32_t bg_color);