[bits 16]

; VGA screen is 80x25

print_splash:
    mov dh, 5
    mov dl, 0
    call move_cursor

    mov bx, msg_splash
    call print_bios
    ret


msg_splash: db '           __                 _____ ____', 0x0D, 0x0A
            db '          / /_()_____  __  __/ __ / ___/', 0x0D, 0x0A
            db '         / __/ / __ \/ / / / / / \__ \  ', 0x0D, 0x0A
            db '        / /_/ / / / / /_/ / /_/ ___/ /  ', 0x0D, 0x0A
            db '        \__/_/_/ /_/\__, /\____/____/   ', 0x0D, 0x0A
            db '                   /____/               ', 0x0D, 0x0A,0x0D, 0x0A
            db '        16-bit Real Mode', 0x0D, 0x0A, 0