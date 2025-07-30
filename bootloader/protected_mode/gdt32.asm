; 8 byte entries for the GDT

gdt_start:

; First entry
; GDT Null Descriptor (mandatory)
gdt_null:
    dq 0x0

; GDT Code Segment Descriptor
; Base=0, Limit=4GB, Ring 0, Executable, Readable
gdt_code:
    dw 0xFFFF    ; Limit (bits 0-15)
    dw 0x0000    ; Base (bits 0-15)
    db 0x00      ; Base (bits 16-23)
    db 0x9A      ; Access Byte: P=1, DPL=00, S=1, Type=Code, A=0, R=1, C=0
    db 0xCF      ; Flags (G=1, D=1) and Limit (bits 16-19)
    db 0x00      ; Base (bits 24-31)

; GDT Data Segment Descriptor
; Base=0, Limit=4GB, Ring 0, Writable
gdt_data:
    dw 0xFFFF       ; Limit (bits 0-15)
    dw 0x0000       ; Base (bits 0-15)
    db 0x00         ; Base (bits 16-23)
    db 0b10010010   ; Access Byte: P=1, DPL=00, S=1, E=0, DC=0, RW=1, A=0
    db 0b11001111   ; Flags (G=1, D=1) and Limit (bits 16-19) (L=0, disabled for 32-bit protected-mode)
    db 0x00         ; Base (bits 24-31)

gdt_end:

; GDT Descriptor 
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of the GDT
    dd gdt_start               ; Start address of the GDT

; Segment Selector (segment offsets)
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start