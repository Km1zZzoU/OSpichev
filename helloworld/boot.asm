[BITS 16]
[ORG 0x7C00]

start:
    xor si, si
    mov ds, si
    mov si, 0x7C00
    mov di, 0x0007
    mov es, di
    mov di, 0xfd00
    mov cx, 0x200
    rep movsb

    jmp 0x781f:0xfff0

    mov si, msg
    call print_string

    jmp $

print_string:
    mov ah, 0x0E
.next_char:
    lodsb
    cmp al, 0
    je done
    int 0x10
    jmp .next_char
done:
    ret

msg db 'Hello, World!', 0

times 510-($-$$) db 0
dw 0xAA55
