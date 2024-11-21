section .text
extern __trap_handler
extern __debug_print_esp
%macro TRAP_WITH_ERROR_CODE 1
[GLOBAL __trap_%1]
__trap_%1:
    push 0b%1
    jmp collect_context
%endmacro

%macro TRAP_WITHOUT_ERROR_CODE 1
[GLOBAL __trap_%1]
__trap_%1:
    push 0x42
    push 0b%1
    jmp collect_context
%endmacro

TRAP_WITHOUT_ERROR_CODE 000000
TRAP_WITHOUT_ERROR_CODE 000001
TRAP_WITHOUT_ERROR_CODE 000010
TRAP_WITHOUT_ERROR_CODE 000011
TRAP_WITHOUT_ERROR_CODE 000100
TRAP_WITHOUT_ERROR_CODE 000101
TRAP_WITHOUT_ERROR_CODE 000110
TRAP_WITHOUT_ERROR_CODE 000111
TRAP_WITH_ERROR_CODE 001000
TRAP_WITHOUT_ERROR_CODE 001001
TRAP_WITH_ERROR_CODE 001010
TRAP_WITH_ERROR_CODE 001011
TRAP_WITH_ERROR_CODE 001100
TRAP_WITH_ERROR_CODE 001101
TRAP_WITH_ERROR_CODE 001110
TRAP_WITHOUT_ERROR_CODE 001111
TRAP_WITHOUT_ERROR_CODE 010000
TRAP_WITH_ERROR_CODE 010001
TRAP_WITHOUT_ERROR_CODE 010010
TRAP_WITHOUT_ERROR_CODE 010011
TRAP_WITHOUT_ERROR_CODE 010100
TRAP_WITH_ERROR_CODE 010101
TRAP_WITHOUT_ERROR_CODE 010110
TRAP_WITHOUT_ERROR_CODE 010111
TRAP_WITHOUT_ERROR_CODE 011000
TRAP_WITHOUT_ERROR_CODE 011001
TRAP_WITHOUT_ERROR_CODE 011010
TRAP_WITHOUT_ERROR_CODE 011011
TRAP_WITHOUT_ERROR_CODE 011100
TRAP_WITH_ERROR_CODE 011101
TRAP_WITH_ERROR_CODE 011110
TRAP_WITHOUT_ERROR_CODE 011111
TRAP_WITHOUT_ERROR_CODE 100000
TRAP_WITHOUT_ERROR_CODE 100001

TRAP_WITHOUT_ERROR_CODE 1000010 ; -> 0x42;

collect_context:
    push ds
    push es
    push fs
    push gs
    pusha

    mov eax, 0x10
    mov ds, ax
    mov es, ax

    push esp
    call __trap_handler
    pop esp

    popa
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 8
;    jmp __debug_print_esp
    IRETD

