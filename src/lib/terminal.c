#include "std_io.h"
#include "keyboard.h"
#include "tools.h"

void term_print_header() {
	io_print(" ******************************************************************************\n");
	io_print(" * |------------------------------------------------------------------------| *\n");
	io_print(" * |                         SELAMAT DATANG DI G-OS                         | *\n");
	io_print(" * |------------------------------------------------------------------------| *\n");
	io_print(" ******************************************************************************\n");
}

void term_print_prefix() {
    io_print("\n [ G-OS ]> ");
}


unsigned char dataIn;
unsigned char cmdBuffer[100];
unsigned int iCmd=0;

void exec_command() {
    if (strcmp(cmdBuffer, "cls") == 0) {
        io_clear();
        term_print_header();
    }
	else {
		io_print("\n unknown command");
	}

    iCmd=0;
	cmdBuffer[iCmd] = 0;
}

void initialize_terminal() {
    io_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE));
	io_clear();
	
	term_print_header();
    term_print_prefix();

    while (1)
    {
        dataIn = waitkey();
        switch (dataIn)
        {
        case 8:
			if (iCmd == 0) break;
			iCmd--;
			cmdBuffer[iCmd] = 0;
			io_putchar(dataIn);
			break;

		case 10:
			exec_command();
            term_print_prefix();
			break;

		default:
			if(iCmd >= 100) break;
			cmdBuffer[iCmd] = dataIn;
			iCmd++;
			cmdBuffer[iCmd] = 0;
			io_putchar(dataIn);
			break;
        }
    }
}