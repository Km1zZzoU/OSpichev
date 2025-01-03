[bits 16]
vbe_set_mode:
    shr cx, 4
    mov es, cx
    push es
    jmp .set_mode

	mov ax, 0x4F00				; get VBE BIOS info
	mov di, vbe_info_block
	int 0x10
	push ax
	mov ax, 0xe00 + '!'
    int 0x10
    pop ax

	cmp ax, 0x4F				; BIOS doesn't support VBE?
	jne .error

	mov ax, word[vbe_info_block.video_modes]
	mov [.offset], ax
	mov ax, word[vbe_info_block.video_modes+2]
	mov [.segment], ax
	mov fs, ax
	mov si, word[.offset]

.find_mode:
	mov dx, [fs:si]
	add si, 2
	mov word[.offset], si
	mov word[.mode], dx
	xor ax, ax
	mov fs, ax

	cmp word[.mode], 0xFFFF			; end of list?
	je .error

    push ax
    mov ax, word[.mode]
    call print_hex
    mov ax, 0xe00 + '|'
    int 0x10
    pop ax
    cmp word[.mode], 0x0100
    jne .next_mode

.set_mode:
	pop es
	push es
	mov ax, 0x4F01				; get VBE mode info
;	mov cx, word[.mode]
    mov cx, 0x011F
	mov di, mode_info_block
	int 0x10

	cmp ax, 0x4F
	jne .error


	; If we make it here, we've found the correct mode!
	; Set the mode
	pop es
	push es
	mov ax, 0x4F02
	mov bx, 0x011F
	or bx, 0x4000	; enable LFB
	xor di, di      ; not sure if some BIOSes need this... anyway it doesn't hurt
	int 0x10

	cmp ax, 0x4F
	jne .error

    pop es
	ret

.next_mode:
	mov ax, word[.segment]
	mov fs, ax
	mov si, word[.offset]
	jmp .find_mode

.error:
    jmp $
	ret

.segment:			dw 0
.offset:			dw 0
.mode:				dw 0

vbe_info_block:
    dd 'VBE2'                  ; Подпись
    dw 0x0100                  ; Версия VESA
    dd 0
    dd 0
    .video_modes: dd 0             ; Указатель на список поддерживаемых видеорежимов
    dw 0
    dw 0
    dd 0
    dd 0
    dd 0
    dw 0

mode_info_block:
    dw 0    ;2
    db 0    ;3
    db 0    ;4
    dw 0    ;6
    dw 0    ;8
    dw 0    ;10
    dw 0    ;12
    dd 0    ;16
    dw 0    ;18
    dw 0    ;20
    dw 0    ;22
    db 0    ;23
    db 0    ;24
    db 0    ;25
    db 0    ;26
    db 0    ;27
    db 0    ;28
    db 0    ;29
    db 0    ;30
    db 0    ;31
    db 0    ;32
    db 0    ;33
    db 0    ;34
    db 0    ;35
    db 0    ;36
    db 0    ;37
    db 0    ;38
    db 0    ;39
    db 0    ;40
    .framebuffer: dd 0x0        ; Физический адрес линейного буфера
    dd 0
    dw 0
    db 0

print_hex:
    push ax                 ; Сохранить значение ax на стеке
    push bx                 ; Сохранить значение bx на стеке
    push cx                 ; Сохранить значение cx на стеке
    push dx                 ; Сохранить значение dx на стеке

    mov cx, 4               ; Установить количество цифр (4 для 16-битного числа)
    mov bx, 0               ; Обнулить bx (используется для индексации)

print_loop:
    shl ax, 4               ; Сдвинуть число влево на 4 бита
    mov dx, ax              ; Копировать значение ax в dx
    and dx, 0x0F            ; Извлечь младшие 4 бита (цифру)
    add dl, '0'             ; Преобразовать в ASCII
    cmp dl, '9'             ; Проверить, является ли цифра > 9
    jbe store_digit         ; Если <= 9, перейти к сохранению
    add dl, 7               ; Если > 9, преобразовать в букву (A-F)

store_digit:
    ; Сохранить цифру в буфер (в обратном порядке)
    mov [hex_digits + bx], dl
    inc bx                  ; Увеличить индекс
    loop print_loop         ; Повторять, пока cx не станет 0

    ; Вывести строку на экран
    mov ah, 0x0E           ; Функция BIOS для вывода символа
    mov cx, 4               ; Количество символов для вывода
    mov bx, 0               ; Обнулить bx для индексации

print_output:
    mov al, [hex_digits + bx] ; Получить символ из буфера
    int 0x10                ; Вывести символ
    inc bx                  ; Увеличить индекс
    loop print_output       ; Повторять, пока cx не станет 0

    pop dx                  ; Восстановить значение dx
    pop cx                  ; Восстановить значение cx
    pop bx                  ; Восстановить значение bx
    pop ax                  ; Восстановить значение ax
    ret

section .data
    hex_digits db '0123456789ABCDEF' ; Символы для шестнадцатеричного вывода
