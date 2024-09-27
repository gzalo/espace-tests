MEMORY
{
    ram : ORIGIN = 0x80000000, LENGTH = 0x8000000
}

SECTIONS
{
    .text : {
		*(.text)
		*(.text*)
	} > ram
    . = ALIGN(4);
	.data : {
		*(.data);
		*(.data*);
    } > ram
    . = ALIGN(4);
    .rodata : {
        *(.rodata);
        *(.rodata*);
    } > ram
    . = ALIGN(4);
    .bss : {
		__bss_start__ = .;
		*(.bss);
		*(.bss*);
		*(COMMON);
		__bss_end__ = .;
	    . = ALIGN(4);
       	end = .;
       	__end__ = .;
    } > ram
}
