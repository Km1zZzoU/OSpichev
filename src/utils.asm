%include "src/traps.asm"
%include "src/testdefaulthandler.asm"

[GLOBAL __loop]
[GLOBAL __sti]
[GLOBAL __inb]
[GLOBAL __outb]
[GLOBAL __load_idt]
section .text

__loop:
    jmp $

__sti:
    sti
    ret

__inb:
    mov dx, word [esp + 4]
    in al, dx
    ret

__outb:
    mov al, 0x42
    mov dx, 0x80
    out dx, al
    mov al, byte [esp + 8]
    mov dx, word [esp + 4]
    out dx, al
    ret

__load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret