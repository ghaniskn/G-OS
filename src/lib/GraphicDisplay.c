#include <stdint.h>
#include "GraphicDisplay.h"
#include "tools.h"
#include "font.h"

const uint16_t video_width = 1024;
const uint16_t video_height = 768;

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA  0x01CF
#define VBE_DISPI_INDEX_ID              0x0
#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2
#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4
#define VBE_DISPI_INDEX_BANK            0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
#define VBE_DISPI_INDEX_X_OFFSET        0x8
#define VBE_DISPI_INDEX_Y_OFFSET        0x9

#define VBE_DISPI_DISABLED              0x00
#define VBE_DISPI_ENABLED               0x01
#define VBE_DISPI_GETCAPS               0x02
#define VBE_DISPI_8BIT_DAC              0x20
#define VBE_DISPI_LFB_ENABLED           0x40
#define VBE_DISPI_NOCLEARMEM            0x80

void vbe_write(uint16_t index, uint16_t value) {
   outw(VBE_DISPI_IOPORT_INDEX, index);
   outw(VBE_DISPI_IOPORT_DATA, value);
}

void vbe_set(uint16_t xres, uint16_t yres, uint16_t bpp) {
   vbe_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
   vbe_write(VBE_DISPI_INDEX_XRES, xres);
   vbe_write(VBE_DISPI_INDEX_YRES, yres);
   vbe_write(VBE_DISPI_INDEX_BPP, bpp);
   vbe_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);   
}

uint32_t get_colorCode(uint8_t red, uint8_t green, uint8_t blue) {
    return blue | (green << 8) | (red << 16);
}

void disp_putPixel(uint16_t x, uint16_t y, uint32_t colorcode) {
    video_framebuffer[x + y * video_width] = colorcode;
}

void disp_setBGColor(uint32_t colorcode) {
    background_color = colorcode;
    for(int i=0; i < 1024 * 768; i++){
		video_framebuffer[i] = background_color;
	}
}

void disp_drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t colorcode) {
    float grad = (float)(y2-y1)/(x2-x1);
    float c = y1 - grad*x1;
    for (uint16_t i=x1; i < x2; i++) {
        disp_putPixel(i, (grad*i + c), colorcode);
    }
}

void disp_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t colorcode) {
    for (uint16_t i=y; i < (y+h); i++) {
        for (uint16_t j=x; j < (x+w); j++) {
            disp_putPixel(j, i, colorcode);
        }
    }
}

void disp_drawChar(uint8_t c, uint16_t x, uint16_t y, uint32_t fg_color, uint32_t bg_color) {
    uint16_t charPosition = (c - 32) * 9 * 16;
    uint8_t row=0;
    uint8_t col=0;
    for (int i = 0; i < 9*16; i++) {
        switch (font_bitmap[charPosition+i]) {
            case '_':
                disp_putPixel(x+col, y+row, bg_color);
                col++;
                break;

            case 'X':
                disp_putPixel(x+col, y+row, fg_color);
                col++;
                break;

            case ',':
                row++;
                col=0;
                break;
        }
    }
}

void disp_check() {
    while (1) {
		for (int i=0; i<256; i++) disp_setBGColor(get_colorCode(255,i,0));
		for (int i=0; i<256; i++) disp_setBGColor(get_colorCode(255-i,255,0));
		for (int i=0; i<256; i++) disp_setBGColor(get_colorCode(0,255,i));
		for (int i=0; i<256; i++) disp_setBGColor(get_colorCode(0,255-i,255));
		for (int i=0; i<256; i++) disp_setBGColor(get_colorCode(i,0,255));
		for (int i=0; i<256; i++) disp_setBGColor(get_colorCode(255,0,255-i));
	}
}