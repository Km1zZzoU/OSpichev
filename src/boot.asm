[BITS 16]
cli

mov ax, 0x07c0
mov ds, ax

mov sp, 0x1000
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
   
    mov cx, 0x200
    rep movsb

    jmp 0x7fe0:0+end_copy_vbr
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


msg db '  ____   _____              _', 10, 13
db ' / __ \ / ____|     (*)    | |', 10, 13
db '| |  | | (___  _ __  _  ___| |__   _____   __', 10, 13
db '| |  | |\___ \|  _ \| |/ __|  _ \ / _ \ \ / /', 10, 13
db '| |__| |____) | |_) | | (__| | | |  __/\ V /', 10, 13
db ' \____/ \____/| .__/|_|\___|_| |_|\___| \_/', 10, 13
db '              | |', 10, 13
db '              |_|', 10, 13, 0
times 510-($-$$) db 0
dw 0xAA55



