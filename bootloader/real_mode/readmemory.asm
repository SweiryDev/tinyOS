[bits 16]

detect_memory:
    xor ebx, ebx            ; Start with EBX = 0
    mov edi, E820_MAP_ADDR  ; Buffer to store entries
    mov word [E820_COUNT_ADDR], 0 ; Initialize entry count to 0

.loop:
    mov eax, 0xE820         ; E820 function
    mov edx, 0x534D4150     ; 'SMAP' signature
    mov ecx, 24             ; Request 24 bytes for the entry
    mov dword [edi + 20], 1 ; Force ACPI 3.0 extended entry type
    int 0x15                ; Call BIOS
    jc .error               ; If carry is set, there was an error

    cmp eax, 0x534D4150     ; Check for 'SMAP' signature on return
    jne .error

    test ebx, ebx           ; If EBX is 0, we are done
    jz .done

    inc word [E820_COUNT_ADDR] ; Increment entry count
    add edi, 24             ; Move to the next 24-byte entry slot
    jmp .loop

.done:
    ret

.error:
    mov bx, msg_e820_fail
    call print_bios
    jmp $

msg_e820_fail db 'E820 memory detection failed!', 0


