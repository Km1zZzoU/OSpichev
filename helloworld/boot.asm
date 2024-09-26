[BITS 16]
cli

mov ax, 0x07c0
mov ds, ax

mov sp, 1
mov ss, sp
xor sp, sp

sti

copy_vbr:
; mov out ds:si (ds*16+si)
    xor si, si

; mov to  es:di (es*16+di)
    mov di, 0x7fe0
    mov es, di
    xor di, di

;0x7fe0:0
; ^ ^ ^
; | | | 
;0x07c0:0
   
    mov cx, 0x0200
    rep movsb

    jmp 0x7fe0:0x0000+end_copy_vbr
end_copy_vbr:
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
