; our abi for 16bit
; ax,bx,cx,dx,dp - можно менять, не рассчитываем, что там лежит то, что нам надо
; si, di - должны не меняться от вызовов макросов и функций
; с остальными вроде очев
[BITS 16]           ; Указание ассемблеру, что код будет выполняться в 16-битном режиме.
jmp _start

;ax bx cx
copy:; копируем участок памяти, начинающийся с %1:0 в %2:0 длинной %3 байт
    push si
    push di
    push ds

; mov out ds:si (ds*16+si)
    mov ds, ax
    xor si, si
; в сегментные регистры нельзя писать
; поэтому пузырим черех другие
    mov es, bx
    xor di, di
; mov to  es:di (es*16+di)

    rep movsb ;в cx колиество байт

    pop ds
    pop di
    pop si
    ret

read_chs:; ax - номер цилиндра, bx - СЕГМЕНТ, того куда пишем (смещение=0)
    push si
    xor si, si      ; устанавливаем номер попытки
.new_attempt:
    inc si
    cmp si, 25
    jg all_bad

    mov ch, al      ; номер цилиндра
    mov cl, 1       ; номер первого сектора

    mov ah, 0x2     ; номер функции
    mov al, 18      ; количесто секторов

;в bx лежит нужный СЕГМЕНТ
    mov es, bx      ; СЕГМЕНТ того, куда пишем (0xadres:0)
    xor bx, bx      ; --------------------------------^-
;из комбинации es и bx получаем адрес

    xor dh, dh      ; номер головки

    int 0x13        ; номер прерывания
    jc .new_attempt

    mov bx, SIZE_9KB; 9 килобайт

    mov ah, 0x2     ; номер функции       ;
    mov al, 18      ; количесто секторов  ;

    inc dh          ; меняем головку

    int 0x13        ; номер прерывания
    jc .new_attempt

    pop si
    ret

load_chs: ; ax - номер цилиндра, bx - СЕГМЕНТ, того куда копируем (смещение=0)
    push bx;<<----------------------------------|
    ;mov ax ax    ; в ax и так то че надо      ;|
    mov bx, BUFF  ; СЕГМЕНТ того куда читаем   ;|
    call read_chs ; читаем сектор в буффер     ;|
                                               ;|
    mov ax, BUFF                               ;|
    pop bx;<<-----------------------------------|
    mov cx, SIZE_18KB
    call copy

    ret

_start:
    cli

    mov sp, 0x0500    ; в ss нельзя закинуть сразу 0х1400, тк тогда sp=0
    mov ss, sp        ; а тк стек растет вниз, мы попадаем в забавную ситуацию
    mov sp, 0xf000    ; такая комбинация даст стек на 0х14000

    mov ax, 0x07c0    ; Загружаем значение 0x07C0 в регистр AX.
    mov ds, ax        ; Устанавливаем сегмент данных (DS) на 0x07C0 (начало загрузочного сектора).

    xor si, si        ; итератор по цилиндрам
    mov di, START_SEG
.start_load:

    mov  ax, si      ;аргумент 1
    mov  bx, di      ;аргумент 2
    call load_chs
    inc  si
    add  di, SIZE_18KB / 16; танцы с бубном так как di - сегмент
    cmp  si, COUNT_CIL
    jl   .start_load ; if (si < COUNT_CIL) { goto start_load }

    mov ax, 0x1420
    mov bx, 0x0800
    mov cx, 0x0200
    call copy

    mov cx, 0x8000
    call cx
    lgdt [gdt_descriptor]

    mov  eax, cr0
    or   al,  1
    mov  cr0, eax


    jmp CODE_SEG:tramplin + 0x7c00
[bits 32]
tramplin:
    mov esp, START_PTR ; настраиваем стек
    mov bx,  DATA_SEG
    mov ss,  bx
    mov es,  bx
    mov ds,  bx
    mov fs,  bx
    mov gs,  bx
    jmp CODE_SEG:0x14400
    ; Welcome to the C!
[bits 16]


all_bad:
    add ax, 0xe00 ;отловить ошибку от int_0x13(0x2)
    int 0x10
    jmp $


gdt_start:
    dq 0x0
gdt_code:
;             GDXU&lim2
      ;адрес 3;       ;acces  ;       адрес 1&2       ;limit 1;
    dq 0000000011001111100110100000000000000000000000001111111111111111b
    ;                      ^ diff with data
    ; 1)00000000     2)0100 1111
    ; base3 1 byte     GDXU limit2
    ; 3)10011010      4)00000000
    ; acces byte        адрес 2
    ; 1 - активен ли
    ; 00 - уровень привелегии
    ; 1 так надо
    ; 1 - сегмент кода
    ; 0 - направление (база меньше лимита)
    ; 1 - доступ на чтение (писать нельзя)
    ; 0 - 0 при создании (так надо)
    ;
    ; 0000000000000000 - адрес 1
    ;
    ; 1111111111111111 - limit 1
gdt_data:
;             GDXU&lim2
      ;base3  ;       ;acces  ;      base 1&2         ;limit 1;
    dq 0000000011001111100100100000000000000000000000001111111111111111b
    ;                      ^ diff with code
    ; 1)00000000     2)0100 1111
    ; base3 1 byte     GDXU limit2
    ; 3)10011010      4)00000000
    ; acces byte        адрес 2
    ; 1 - активен ли
    ; 00 - уровень привелегии
    ; 1 так надо
    ; 1 - сегмент кода
    ; 0 - направление (база меньше лимита)
    ; 1 - доступ на запись (читать нельзя)
    ; 0 - 0 при создании (так надо)
    ;
    ; 0000000000000000 - адрес 1
    ;
    ; 1111111111111111 - limit 1

gdt_descriptor:
    dw $ - gdt_start - 1
    dd gdt_start + START_PTR

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
START_SEG equ 0x1400; СГЕМЕНТ того, где начинается OS
START_PTR equ 0x14000; АДРЕС  того, где начинается OS
; 0х14000 тк так получается довольно выровненный адрес + целое и четное число цилиндров до 0х80000
COUNT_CIL equ 24      ; (0x80000 - 0x14000) / size_18kb
BUFF      equ 0x100   ; СЕГМЕНТ буфера, НЕ адрес
SIZE_9KB  equ 0x2400
SIZE_18KB equ 0x4800

times 510-($-$$) db 0     ; Добавляем нули, чтобы заполнить до 510 байт (размер загрузочного сектора 512 байт).
dw 0xAA55                 ; Магическое число завершения загрузочного сектора, которое BIOS использует для проверки загрузочного сектора.