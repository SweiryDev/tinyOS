; --- 64-bit print function ---
print_string_64:
    mov rdx, VGA_MEMORY_ADDRESS
    .loop:
        mov al, [rdi]
        cmp al, 0
        je .done
        mov ah, BW_STYLE 
        mov [rdx], ax
        add rdi, 1
        add rdx, 2
        jmp .loop
    .done:
        ret

; Address and style defined in print32.asm