MEMORY
{
    ram : ORIGIN = 0x80000000, LENGTH = 0x8000000
}

SECTIONS
{
	.text : {
		*(.text*);
        *(.rodata*);
		*(.data*);
		*(.bss.*);
     } > ram

}
