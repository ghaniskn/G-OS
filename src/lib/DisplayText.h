#include <stdint.h>

void get_dispPos(uint16_t x, uint16_t y, uint16_t *x_target, uint16_t *y_target);

void clearScreen();
void setCursor(uint16_t x, uint16_t y);
void newLine();
void putChar_withBG(uint8_t c, uint32_t fg_color, uint32_t bg_color);
void putChar(uint8_t c, uint32_t fg_color);
void print(const char* data);