ENTRY(start)
SECTIONS {
    . = 1M;                  /* GRUB carga el kernel a partir de 1 MB */
    .text : { *(.multiboot) *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON) }
}
