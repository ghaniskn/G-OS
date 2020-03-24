#include <stdint.h>
#include "DisplayText.h"
#include "GraphicDisplay.h"
#include "tools.h"

uint16_t cursor_y;
uint16_t cursor_x;
uint16_t widht = 128;
uint16_t height = 96;

void get_dispPos(uint16_t x, uint16_t y, uint16_t *x_target, uint16_t *y_target) {
    *x_target = x*8;
    *y_target = y*16;
}

void clearScreen() {
    disp_setBGColor(background_color);
    setCursor(0,0);
}

void setCursor(uint16_t x, uint16_t y) {
    cursor_x = x;
    cursor_y = y;
}

void newLine() {
	cursor_y++;
	cursor_x=0;
	
	setCursor(cursor_x,cursor_y);
}

void putChar_withBG(uint8_t c, uint32_t fg_color, uint32_t bg_color) {
    if (c == '\n') newLine();

    else if (c == 8) {
		if(cursor_x != 0) {
			cursor_x--;
		} else {
			if (cursor_y == 0) return;

			cursor_x = 79;
			cursor_y--;
		}
        uint16_t x_text;
        uint16_t y_text;
        get_dispPos(cursor_x, cursor_y, &x_text, &y_text);
        disp_drawChar(' ', x_text, y_text, 0x00000000, background_color);
	}

    else {
        uint16_t x_text;
        uint16_t y_text;
        get_dispPos(cursor_x, cursor_y, &x_text, &y_text);
        disp_drawChar(c, x_text, y_text, fg_color, bg_color);
        if (++cursor_x == widht) {
			cursor_x = 0;
			if (cursor_y == height)
				cursor_y = 0;
		}
    }
}

void putChar(uint8_t c, uint32_t fg_color) {
    putChar_withBG(c, fg_color, background_color);
}

void print(const char* data) {
    for (uint16_t i = 0; i < strlen(data); i++) putChar(data[i], get_colorCode(255,255,255));
}