#include "std_io.h"

void Div0ExceptionHandler(){
	asm("pusha");
	io_print("Exception: Division By Zero \n");
	asm("hang: jmp hang");
	asm("popa");
	asm("leave");
	asm("iret");
}
