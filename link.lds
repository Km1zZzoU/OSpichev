ENTRY(__start__)
OUTPUT_FORMAT(binary)

SECTIONS {
    . = 0x14400;

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
    
    .rodata : {
        *(.rodata)
    }
}