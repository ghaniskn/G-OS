.intel_syntax noprefix
.extern KMain
.global multiboot_entry

.text
	.align 8
	multiboot_header_start:
		.long 0xE85250D6
		.long 0
		.long multiboot_header_end - multiboot_header_start
		.long -(0xE85250D6+0+(multiboot_header_end - multiboot_header_start))
		.align 8

		address_tag:
			.short	2
			.short	0
			.long	24
			.long	multiboot_header_start
			.long	text
			.long	bss
			.long	end
		.align 8

		entry_tag:
			.short	3
			.short	0
			.long	12
			.long	multiboot_entry
			.align 8

		framebuffer_tag:
			.short	5
			.short	0
			.long	20
			.long	1024
			.long	768
			.long	32
		.align 8

		end_tag:
			.short	0
			.short	0
			.long	8
		.align 8
		
	multiboot_header_end:
	
	multiboot_entry:
		push ebx
		push eax
		call KMain
		jmp $
