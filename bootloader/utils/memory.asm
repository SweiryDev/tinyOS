[bits 16]

print_mem_size:
    ; Set cursor position
    mov dh, 15
    mov dl, 0
    call move_cursor

    mov bx, msg_mem
    call print_bios

    ; BIOS call to get RAM memory size (KB)
    int 0x12
    call ax_to_string

    mov bx, di  ; save string pointer to print size
    call print_bios

    mov bx, msg_kb
    call print_bios
    
    ret


; -- Helper Function --
; Convert the number in ax to decimal string
; Input - ax, Output - di (pointer to the string)
ax_to_string:
    pusha ; save state
    mov cx, 10  ; divisor
    mov bx, 0   ; character counter

.convert_loop:
    xor dx, dx  ; clear dx
    div cx      ; ax division by cx, quotient in ax, remainder in dx
    add dl, '0' ; Convert remainder to ASCII
    push dx     ; Push digit to the stack
    inc bx      ; Inc count
    cmp ax, 0   ; quotient zero?
    jne .convert_loop

    ; build the string from the stack
    mov di, num_buffer ; point di to the string buffer

.pop_loop:
    pop ax          ; ASCII digit to al
    mov [di], al    ; move to the buffer
    inc di          ; next character
    dec bx          ; dec counter
    cmp bx, 0       ; done with digits ?
    jne .pop_loop

    mov byte[di], 0 ; add null terminator 
                    ; to end the string
    popa ; restore state
    mov di, num_buffer ; pointer to start
    ret

; String buffer of the converted number
num_buffer times 10 db 0 

msg_mem db 'Memory Size: ', 0
msg_kb db ' KB', 0 
