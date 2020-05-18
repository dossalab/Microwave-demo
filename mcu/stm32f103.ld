MEMORY
{
	rom(rx) : ORIGIN = 0x08000000, LENGTH = 64K
	ram(rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

STACK_SIZE = 2K;
ENTRY(reset_handler)

SECTIONS
{
	. = ORIGIN(rom);

	.text : {
		KEEP(*(.isr_vectors))
		. = ALIGN(4);
		*(.text*)
		. = ALIGN(4);
		*(.rodata*)
		. = ALIGN(4);
		__text_end__ = .;
	} >rom

	. = ORIGIN(ram);

	__data_load__ = LOADADDR(.data);

	/* Stack is placed before ram, that should be safer */
	.stack (NOLOAD) : {
		. = ALIGN(8);
		. = . + STACK_SIZE;
		. = ALIGN(8);
		__stack__  = .;
	} > ram

	.data : {
		__data_start__ = .;
		*(.data*)
		. = ALIGN(4);
		__data_end__ = .;
	} >ram AT >rom

	.bss (NOLOAD) : {
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
	} >ram

	. = ALIGN(4);
	end = .;
}

