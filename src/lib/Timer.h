#include <stdint.h>

void SetPIC(char IRQ0, char IRQ8);
void SetPIT(uint16_t Freq);
void Timer_initialize(uint16_t freq);
void sleep(uint32_t ms);