section .text

extern __handler

[GLOBAL context_handler]
context_handler:
    push gs
    push fs
    push es
    push ds
    pusha

    mov eax, 0x10
    mov ds, ax
    mov es, ax

    push esp
    call __handler
    pop esp

    popa
    pop ds
    pop es
    pop fs
    pop gs
    add esp, 8

    IRETD