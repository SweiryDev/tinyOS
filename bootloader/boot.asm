; BIOS load the boot sector at 0x7C00
; [org 0x7C00]
; Now handled by the linker file

; == Sector 1 ==
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
    mov cx, 0x0045  ; load sectors (Sector #2 - Kernel)
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
%include "real_mode/load.asm"
%include "real_mode/print.asm"
%include "real_mode/splash.asm"

boot_drive db 0x00
times 510 - ($ - $$) db 0x00 ; Pad with zeros 
dw 0xAA55 ; Magic number for the BIOS
; == End Sector 1 == 

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

    ; Wait for the user to continue
    call wait_for_key

    ; Proceed to 32-bit protected mode boot
    call clear_screen

    ; The switch will load protected mode,
    call switch_to_protected_mode

%include "real_mode/time.asm"
%include "real_mode/memory.asm"
%include "protected_mode/gdt32.asm"
%include "protected_mode/switch32.asm"
%include "protected_mode/print32.asm"
%include "protected_mode/paging32.asm"

msg_press_to_boot db 'Press Enter to boot (or another key to refresh)...', 0
msg_protected_mode db '32-bit Protected Mode Loaded! Paging enabled', 0
times 512 - ($ - bootsector_2) db 0x00
; == End Sector 2 == 

; == Sector 3 ==
[bits 32]

; This sector is loaded after the transition
; to protected mode (32-bit code)
bootsector_3:
    ; Printing is done by changing the VGA memory directly
    ; (no BIOS interrupts)

    ; Setup paging for the virtual memory
    call setup_paging

    ; Print msg in 32-bit mode (print32.asm)
    mov ebx, msg_protected_mode
    call print_string_32

    ; Switch to 64-bit long mode and enable paging
    call switch_to_long_mode


[bits 64]
long_mode_sector:
    ; Print msg in 64-bit mode (print64.asm)
    mov rdi, msg_long_mode
    call print_string_64

    ; External kernel_start function (entry.asm)
    extern kernel_start

    ; Jump to kernel
    call kernel_start

%include "long_mode/gdt64.asm"
%include "long_mode/switch64.asm"
%include "long_mode/print64.asm"

msg_long_mode: db 'Switched to 64-bit Long Mode!', 0
; times 510 - ($ - bootsector_3) db 0x00
; == End Sector 3 ==
