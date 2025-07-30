[bits 32]

; Final switch to 64-bit long mode
switch_to_long_mode:
    ; Prepare long mode and paging
    ; Load the address of PML4T into cr3
    mov eax, PML4T
    mov cr3, eax

    ; Enable Physical Address Extension (PAE)
    mov eax, cr4
    or eax, 1 << 5  ; bit 5 enable
    mov cr4, eax

    ; Enable Long Mode
    ; Read the EFER Model_Specific Register (MSR)
    mov ecx, 0xC0000080 ; EFER MSR address
    rdmsr               ; read MSR into edx:eax

    ; Set the Long Mode Enable (LME) bit
    or eax, 1 << 8 ; enable bit 8
    wrmsr          ; write the value to the EFER

    ; Enable Paging (will activate 64-bit mode)
    mov eax, cr0
    or eax, 1 << 31 ; set the paging bit
    mov cr0, eax

    ; Load the 64-bit GDT
    lgdt [gdt64_descriptor]

    ; Final jump
    ; Far jump to the 64-bit code segment
    jmp CODE_SEG_64:long_mode_start

[bits 64]
long_mode_start:
    ; Set up data segments
    mov ax, DATA_SEG_64
    mov ds, ax
    mov ss, ax
    mov es, ax

    jmp long_mode_sector
