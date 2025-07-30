[bits 32]

; Print string from address in ebx
print_string_32:
    mov edx, VGA_MEMORY_ADDRESS ; VGA buffer start address
    .loop:
        mov al, byte[ebx]   ; Read character
        cmp al, 0       ; check if done
        je .done
        mov ah, BW_STYLE    ; White on black style
        mov word[edx], ax
        add ebx, 1
        add edx, 2
        jmp .loop
    .done:
        ret

VGA_MEMORY_ADDRESS equ 0xb8000
BW_STYLE equ 0x0F