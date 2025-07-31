; Switch to 32-bit protected-mode
[bits 16]

; Wait for the Enter key
; This will lock the CPU until key is pressed
wait_for_key:
    mov ah, 0x00 ; BIOS wait for key press function
    int 0x16     ; Key in ax

    cmp al, 0x0D ; Enter key code
    jne bootsector_2 ; Update Time for any other key

    ret

switch_to_protected_mode:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load the GDT

    mov eax, cr0
    or eax, 0x1 ; Set the cr0 enable bit
    mov cr0, eax

    jmp CODE_SEG:init_protected_mode

[bits 32]
init_protected_mode:
    ; Set up segment registers for 32-bit env
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    jmp bootsector_3




