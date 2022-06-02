OUTPUT_ARCH( "riscv" )

MEMORY
{
    FLASH(rx)   :   ORIGIN = 0x40050000, LENGTH = 257k
    RAM(rxw)    :   ORIGIN = 0x00010000, LENGTH = 20k
}

ENTRY(Reset_Handler)

SECTIONS
{
    /* The program code and other data goes into FLASH */
    .text :
    {
        . = ALIGN(4);
        __image_copy_start = .;
        KEEP(*(.vectors))
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata*)
    } > FLASH

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
        __image_copy_end = .;
    } > RAM AT > FLASH

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

#if defined(__HEAP_SIZE) && (__HEAP_SIZE > 0)
    .heap (NOLOAD):
    {
        . = ALIGN(4);
        __heap_start = .;
        . += __HEAP_SIZE;
        . = ALIGN(4);
        __heap_end = .;
    } > RAM
#endif

    .stack (NOLOAD):
    {
        . = ALIGN(4);
        __stack = .;
        . += __STACK_SIZE;
        . = ALIGN(4);
        __stack_top = .;
    } > RAM
}
