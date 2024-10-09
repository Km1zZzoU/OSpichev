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

; mov out ds:si (ds*16+si)
    mov ds, ax
    xor si, si
; в сегментные регистры нельзя писать
; поэтому пузырим черех другие
    mov es, bx
    xor di, di
; mov to  es:di (es*16+di)

    rep movsb ;в cx колиество байт

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

print_string:
    mov ah, 0x0E    ; Устанавливаем режим BIOS для вывода символов в текстовом режиме (INT 0x10).
.next_char:
    lodsb           ; Загружаем следующий байт (символ) из [SI] в AL.
    cmp al, 0       ; Сравниваем AL с нулем (нулевой байт означает конец строки).
    je done         ; Если AL равен 0, перейти на метку done.
    int 0x10        ; Вызов прерывания 0x10 для вывода символа на экран.
    jmp .next_char  ; Переход к обработке следующего символа.
done:               ; Метка окончания вывода строки.
    ret             ; Возврат из процедуры.

_start:

    cli                 ; Отключаем прерывания для безопасности при настройке сегментных регистров.

    mov sp, 0x0500    ;в ss нельзя закинуть сразу 0х1400, тк тогда sp=0
    mov ss, sp        ;а тк стек растет вниз, мы попадаем в забавную ситуацию
    mov sp, 0xf000    ;такая комбинация даст стек на 0х14000

    mov ax, 0x07c0      ; Загружаем значение 0x07C0 в регистр AX.
    mov ds, ax          ; Устанавливаем сегмент данных (DS) на 0x07C0 (начало загрузочного сектора).

    sti                 ; Включаем прерывания после инициализации сегментов.

    mov si, msg
    call print_string

    xor si, si        ; итератор по цилиндрам
    mov di, START_SEG ;
.start_load:

    mov ax, si      ;аргумент 1
    mov bx, di      ;аргумент 2
    call load_chs
    inc si

    add di, SIZE_18KB / 16; танцы с бубном так как di - сегмент

    cmp si, COUNT_CIL
    jl .start_load ; if (si < COUNT_CIL) { goto start_load }

    jmp $
    ;jmp 0x1400:0x0200+end_copy_vbr  ; Переход на новый адрес 0x7FE0:0, куда был скопирован код.

all_bad:
    add ax, 0xe00 ;отловить ошибку от int_0x13(0x2)
    int 0x10



msg db '  ____   _____              _', 10, 13
    db ' / __ \ / ____|     (*)    | |', 10, 13
    db '| |  | | (___  _ __  _  ___| |__   _____   __', 10, 13
    db '| |  | |\___ \|  _ \| |/ __|  _ \ / _ \ \ / /', 10, 13
    db '| |__| |____) | |_) | | (__| | | |  __/\ V /', 10, 13
    db ' \____/ \____/| .__/|_|\___|_| |_|\___| \_/', 10, 13
    db '              | |', 10, 13
    db '              |_|', 10, 13, 0

START_SEG equ 0x1400  ; СГЕМЕНТ тоого, где начинается OS
; 0х14000 тк так получается довольно выровненный адрес + целое и четное число цилиндров до 0х80000
COUNT_CIL equ 24      ; (0x80000 - 0x14000) / size_18kb
BUFF      equ 0x100   ; СЕГМЕНТ буфера, НЕ адрес
SIZE_9KB  equ 0x2400
SIZE_18KB equ 0x4800

times 510-($-$$) db 0     ; Добавляем нули, чтобы заполнить до 510 байт (размер загрузочного сектора 512 байт).
dw 0xAA55                 ; Магическое число завершения загрузочного сектора, которое BIOS использует для проверки загрузочного сектора.
