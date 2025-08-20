[bits 32]

global gdt64_start
gdt64_start:

; GDT Null Descriptor
gdt64_null:
    dq 0x0

; GDT 64-bit Code Segment Descriptor
gdt64_code:
    dw 0x0000       ; Limit - Ignored
    dw 0x0000       ; Base (bits 0-15) - Ignored
    db 0x00         ; Base (bits 16-23) - Ignored
    db 0b10011010   ; Access Byte: P=1, DPL=0, S=1, Type=Code, R
    db 0b00100000   ; Flags: L=1 (Long Mode bit enable)
    db 0x00         ; Base (bits 24-31) - Ignored

gdt64_data:
    dw 0x0000       ; Limit - Ignored
    dw 0x0000       ; Base - Ignored
    db 0x00         ; Base - Ignored
    db 0b10010010   ; Access Byte: P=1 DPL=0, S=1, Type=Data, W
    db 0b00000000   ; Flags (zero)
    db 0x00         ; Base - Ignored

; GDT 64-bit User Code Segment Descriptor
gdt64_user_code:
    dw 0x0000       ; Limit - Ignored
    dw 0x0000       ; Base - Ignored
    db 0x00         ; Base - Ignored
    db 0b11111010   ; Access Byte: P=1, DPL=3, S=1, Type=Code, R
    db 0b00100000   ; Flags: L=1 (Long Mode bit)
    db 0x00         ; Base - Ignored

; GDT 64-bit User Data Segment Descriptor
gdt64_user_data:
    dw 0x0000       ; Limit - Ignored
    dw 0x0000       ; Base - Ignored
    db 0x00         ; Base - Ignored
    db 0b11110010   ; Access Byte: P=1, DPL=3, S=1, Type=Data, W
    db 0b00000000   ; Flags (zero)
    db 0x00         ; Base - Ignored

gdt64_tss:
    dw 103          ; Limit (size of TSS -1)
    dw 0            ; Base
    db 0            ; Base
    db 0b10001001   ; Access Byte: P=1, DPL=0, Type=TSS
    db 0            ; Flags and Limit (zero)
    db 0            ; Base
    dq 0            ; Base 

gdt64_end:

; GDT descriptor
gdt64_descriptor:
    dw gdt64_end - gdt64_start - 1 ; Size
    dd gdt64_start             ; Start address 

; Segment Selector
CODE_SEG_64 equ gdt64_code - gdt64_start
DATA_SEG_64 equ gdt64_data - gdt64_start