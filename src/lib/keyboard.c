#include "tools.h"
#include "keyboard.h"

//Ke: Keyboard Encoder
//Kc: Keyboard Controller
//Kb: Keyboard

const char CAPS_ON = 4;
const char NUM_ON = 2;
const char SCROLL_ON = 1;

/* Keyboard Driver */
#define L_SHIFT_DOWN 0x2a
#define L_SHIFT_UP 0xaa
#define R_SHIFT_DOWN 0x36
#define R_SHIFT_UP 0xb6
#define CAPSLOCK_DOWN 0x3a
#define NUMLOCK_DOWN 0x45
#define SCROLLLOCK_DOWN 0x46

#define CTRL_DOWN 0x1d
#define CTRL_UP 0x9d
#define ALT_DOWN 0x38
#define ALT_UP 0xb8

unsigned char keyExtended;
unsigned char keyCode;
unsigned char keyDown;
unsigned char keyUp;
unsigned char keyAsc;

struct STATUS_KEYBOARD{
	unsigned char capsLock;
	unsigned char numLock;
	unsigned char lShift;
	unsigned char rShift;
	unsigned char lCtrl;
	unsigned char lAlt;
	unsigned char rCtrl;
	unsigned char rAlt; 
} kb_stats;

char mapNormal [] = {
	0 ,  0 , '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=',  8 ,  9 , 'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', 10 , 0  ,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`',  0 ,'\\', 'z', 'x', 'c', 'v', 'b', 'n',
	'm', ',', '.', '/',  0 ,  0 ,  0 , ' ',  0 ,  0 ,
	 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
	 0 ,  0 ,  0 ,  0 , '-',  0 ,  0 ,  0 , '+',  0 ,
	 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 , 10 ,  0
};
	 
char mapCapslock[] ={
	0 ,  0 , '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=',  8 ,  9 , 'Q', 'W', 'E', 'R',
	'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 10 , 0  ,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',
	'\'', '`',  0 ,'\\', 'Z', 'X', 'C', 'V', 'B', 'N',
	'M', ',', '.', '/',  0 ,  0 ,  0 , ' ',  0 ,  0 ,
	 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
	 0 ,  0 ,  0 ,  0 , '-',  0 ,  0 ,  0 , '+',  0 ,
	 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 , 10 ,  0
};
	 
char mapShift []  = {
	0 ,  0 , '!', '@', '#', '$', '%', '^', '&', '*',
	'(', ')', '_', '+',  8 ,  9 , 'Q', 'W', 'E', 'R',
	'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 10 , 0  ,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~',  0 , '|', 'Z', 'X', 'C', 'V', 'B', 'N',
	'M', '<', '>', '?',  0 ,  0 ,  0 , ' ',  0 ,  0 ,
	 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
	 0 ,  0 ,  0 ,  0 , '-',  0 ,  0 ,  0 , '+',  0 ,
	 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 , 10 ,  0
};

char mapCtrl []    = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,13, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,13, 0
};


void ke_sendcommand(char Perintah)
{
	while(inb(0x64) & 2);
	outb(0x60,Perintah);
}

void kc_sendcommand(char Perintah)
{
	while(inb(0x64) & 2);
	outb(0x64,Perintah);
}

void kb_setled(char Leds)
{
	ke_sendcommand(0xED);
	ke_sendcommand(Leds);
}

void keyboard_irq()
{
	asm("cli");
	asm("pusha");
	keyCode = inb(0x60);
	//asm("in al,0x60");
	//asm("mov [_keyCode],al");
	
	if (keyCode == 0xE0) {
		keyExtended = 1;
		goto irq_stop;
	}
	keyExtended = 0;
	
	switch(keyCode)
	{
		case L_SHIFT_DOWN:
			kb_stats.lShift = 1;
			goto irq_stop;
		case L_SHIFT_UP:
			kb_stats.lShift = 0;
			goto irq_stop;
		case R_SHIFT_DOWN:
			kb_stats.rShift = 1;
			goto irq_stop;
		case R_SHIFT_UP:
			kb_stats.rShift = 0;
			goto irq_stop;
		case CAPSLOCK_DOWN:
			if(kb_stats.capsLock == 0) kb_stats.capsLock = 1;
			else kb_stats.capsLock = 0;
			goto irq_stop;
		case NUMLOCK_DOWN:
			if(kb_stats.numLock == 0) kb_stats.numLock = 1;
			else kb_stats.numLock = 0;
			goto irq_stop;
	}
	
	if (keyCode >= 0x80)
	{
		keyUp=keyCode;
		goto irq_stop;
	}
	else
	{
		if(kb_stats.capsLock == 1) keyAsc=mapCapslock[(int)keyCode];
		else if(kb_stats.lShift == 1) keyAsc=mapShift[(int)keyCode];
		else if(kb_stats.rShift==1) keyAsc=mapShift[(int)keyCode];
		else keyAsc=mapNormal[(int)keyCode];
		
		keyDown=keyCode;
	}

	irq_stop:
	    asm("mov al, 0x20");
	    asm("outb 0x20, al");
	    asm("popa");
	    asm("leave");
	    asm("sti");
	    asm("iret");
}		

unsigned char waitkey() {
    keyAsc=0;
    loopz:
	    if(keyAsc == 0) goto loopz;
    return keyAsc;
}
