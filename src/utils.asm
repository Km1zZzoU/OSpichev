%include "src/traps.asm"
%include "src/testdefaulthandler.asm"

section .text

[GLOBAL __loop]
__loop:
    jmp $

[GLOBAL __cli]
__cli:
    cli
    ret

[GLOBAL __sti]
__sti:
    sti
    ret

[GLOBAL __inb]
__inb:
    mov dx, word [esp + 4]
    in al, dx
    ret

[GLOBAL __outb]
__outb:
    mov al, 0x42
    mov dx, 0x80
    out dx, al
    mov al, byte [esp + 8]
    mov dx, word [esp + 4]
    out dx, al
    ret

[GLOBAL __load_idt]
__load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

[GLOBAL __setup_paging]
__setup_paging:
    ; mov eax, cr4
    ; or eax, 0x00000010
    ; mov cr4, eax

    pop eax
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

[GLOBAL __eoi]
__eoi:
    mov dx, 0x20
    mov al, 0x20
    out dx, al
    ret