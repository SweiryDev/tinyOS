[bits 16]

; Print function from bx reg
print_bios:
    ; Save state
    push ax
    push bx

    ; Set the BIOS to print mode
    mov ah, 0x0E
    
    print_bios_loop:
        ; Iterate to the end of the string
        cmp byte[bx], 0
        je print_bios_end

        ; Load the byte to print and 
        ; interrupt the bios to print
        mov al, byte[bx]
        int 0x10

        ; Go to the next letter
        inc bx
        jmp print_bios_loop

    print_bios_end:
        ; Restore state
        pop bx
        pop ax

        ret

; Print Hex letters to the screen for bx reg
print_hex_bios:
    ; Save state
    pusha

    ; Print mode
    mov ah, 0x0E

    ; Print prefix '0x'
    mov al, '0'
    int 0x10
    mov al, 'x'
    int 0x10

    ; Counter, 4 nibbles in 16-bit
    mov cx, 4

    print_hex_bios_loop:
        ; Done printing
        cmp cx, 0
        je print_hex_bios_end

        push bx        
        shr bx, 12
        cmp bx, 10
        ; If bx > 10, printing hex 'A-F' digits
        jge print_hex_bios_h
        
        ; Set ASCII letter to '0-9', start at '0'
        mov al, '0'
        add al, bl

        ; End loop
        jmp print_hex_bios_loop_end
    
    print_hex_bios_h:
        ; Set ASCII letter to 'A-F', start at 'A'
        sub bl, 10
        mov al, 'A'
        add al, bl

    print_hex_bios_loop_end:
        ; Print
        int 0x10

        ; Restore bx
        ; Shift to the next 4 bits
        pop bx
        shl bx, 4

        ; Move to next hex letter
        dec cx

        jmp print_hex_bios_loop

    print_hex_bios_end:
        ; Restore state
        popa
        ret


; BIOS set screen function
clear_screen:
    mov ax, 0x03
    int 0x10
    ret

; BIOS turn off cursor
disable_cursor:
    ; Set cursor shape
    mov ah, 0x01
    ; Disable cursor
    mov ch, 0x20
    int 0x10
    ret

; BIOS move cursor to position
; dh - Row, dl - Col
move_cursor:
    mov ah, 0x02 ; BIOS function to set cursor position
    mov bh, 0x00 ; page number 0

    ; Call BIOS to move cursor
    int 0x10

    ret
