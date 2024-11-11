ENTRY(__start__)
OUTPUT_FORMAT(binary)

SECTIONS {
    . = 0x14200;

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