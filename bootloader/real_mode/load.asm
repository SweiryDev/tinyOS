[bits 16]

; Sector start point in bx
; Number of sectors in cx
; Destination address in dx
load_bios:
    ; Save state
    pusha

    ; Save number of sectors
    push cx

    ; Configure read from BIOS
    mov ah, 0x02
    mov al, cl
    mov cl, bl
    mov bx, dx

    ; Using QEMU to emulate cylinder and head
    ; is 0, for real drive this need to be modified
    mov ch, 0x00 ; Cylinder
    mov dh, 0x00 ; Cylinder head

    ; Load the drive to read from boot_drive
    mov dl, byte[boot_drive]
    
    ; Interrupt to trigger BIOS disk read
    int 0x13

    ; carry bit will set if there's error
    ; with the disk load
    jc bios_disk_error

    ; Compare read disk to requested disk to read
    pop bx  ; pop original disk to read
    cmp al, bl
    jne bios_disk_error

    ; Disk is read
    ; Restore state
    popa
    ret

bios_disk_error:
    call clear_screen

    ; Print error message
    mov bx, error
    call print_bios

    ; Error code in ah, print error code
    shr ax, 8 ; shift error code to al
    mov bx, ax
    call print_hex_bios

    jmp $ ; Loop lock

error db `ERROR Loading Sectors Code: `, 0
