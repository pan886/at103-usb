OUTPUT_ARCH( "riscv" )

MEMORY
{
    RAM(rxw)    :   ORIGIN = 0x00010000, LENGTH = 20k
}

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.vectors))
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata*)
    } > RAM

    __etext = ALIGN (4);
    .data :
    {
        . = ALIGN(4);
        __data_start = .;
        *(.data .data.*)
        *(.sdata .sdata.*)
        . = ALIGN(4);
        /* All data end */
        __data_end = .;
    } > RAM

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss*)
        *(.sbss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > RAM

    .stack (NOLOAD):
    {
        . = ALIGN(4);
        __stack = .;
        . += __STACK_SIZE;
        __stack_top = .;
        . = ALIGN(4);
    } > RAM
}
