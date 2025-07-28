; BIOS load the boot sector at 0x7C00
[org 0x7C00]

; x86 starts in 16-bit real-mode, use 16-bit code 
[bits 16]

; Setup
start:
    ; -- Environment Setup --
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00 ; Stack grows downward
    mov bp, sp

    ; Save boot drive ID
    mov byte[boot_drive], dl

    ; Load next sector (sector 1 is already loaded)
    ; Load configuration:
    mov bx, 0x0002  ; load sector #2
    mov cx, 0x0001  ; load 1 sector
    mov dx, 0x7E00  ; load the sector to location 0x7E00
                    ; (512 bytes gap)

    ; Load sector #2
    call load_bios

    ; Clear screen before print
    call clear_screen

    ; Disable cursor
    call disable_cursor

    ; Show Splash screen
    call print_splash

    ; Go to sector 2
    jmp bootsector_2

; Use the -i flag in NASM to include utils folder
%include "load.asm"
%include "print.asm"
%include "splash.asm"

boot_drive db 0x00
times 510 - ($ - $$) db 0x00 ; Pad with zeros 
dw 0xAA55 ; Magic number for the BIOS

; == SECTOR 2 ==
; This is the second sector loaded with
; with load_bios (bx=2)

bootsector_2:
    ; Print clock
    call print_time

    ; Print available RAM (< 1MB)
    call print_mem_size

    ; Move cursor and print boot msg
    mov dh, 20
    mov dl, 0
    call move_cursor
    mov bx, msg_press_to_boot
    call print_bios

    call wait_for_key

; -- Wait for the Enter key --
; This will lock the CPU until key is pressed
wait_for_key:
    mov ah, 0x00 ; BIOS wait for key press function
    int 0x16     ; Key in ax

    cmp al, 0x0D ; Enter key code
    jne wait_for_key 

    ; Proceed to kernel boot (REPLACE IT)
    call clear_screen
    jmp $

%include "time.asm"
%include "memory.asm"


msg_press_to_boot db 'Press Enter to boot...', 0
times 512 - ($ - bootsector_2) db 0x00



