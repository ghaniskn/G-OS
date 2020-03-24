typedef unsigned long long GdtEntry;

struct GdtRegister{
	short limit;
	long   base;
}__attribute__((packed));

GdtEntry gdt[3] = {
	0x0000000000000000,
	0x00CF9A000000FFFF,
	0x00CF92000000FFFF};
struct GdtRegister gdtr;

void GDT_initialize(){
	gdtr.base = (unsigned long)&gdt;
	gdtr.limit = sizeof(gdt);
	asm("lgdt gdtr");
	asm("ljmp 8,jump_here");
	asm("jump_here:");
	asm("mov ax,16");
	asm("mov ds,ax");
	asm("mov es,ax");
	asm("mov fs,ax");
	asm("mov gs,ax");
}