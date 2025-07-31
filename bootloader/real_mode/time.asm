[bits 16]

; Read time from the RTC
; time is coded in BCD
print_time:
    ; Move cursor
    mov dh, 14
    mov dl, 0
    call move_cursor

    ; Print msg
    mov bx, msg_boot_time
    call print_bios

    ; BIOS call to acquire time
    mov ah, 0x02
    int 0x1A

    ; ch - hours
    ; cl - minutes
    ; dh - seconds

    ; Convert hours
    mov al, ch
    call bcd_to_ascii
    mov bx, ax
    call print_two_chars

    ; Separator
    mov al, ':'
    call print_char

    ; Convert minutes
    mov al, cl
    call bcd_to_ascii
    mov bx, ax
    call print_two_chars

    ; Separator
    mov al, ':'
    call print_char

    ; Convert seconds
    mov al, dh
    call bcd_to_ascii
    mov bx, ax
    call print_two_chars

    ret

; -- Helper Functions --
; Input - al (BCD) , output - ax (ASCII)
bcd_to_ascii:
    push cx ; save state
    mov ah, al

    shr ah, 4   ; mask
    add ah, '0' ; convert to ASCII 

    and al, 0x0F ; mask
    add al, '0'  ; convert to ASCII

    pop cx ; restore state
    ret

print_char:
    mov ah, 0x0E
    int 0x10
    ret

print_two_chars:
    push ax
    mov al, ah ; print high character first
    call print_char
    pop ax     ; char in al
    call print_char
    ret

msg_boot_time db 'Last Update Time: ', 0