[bits 32]

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

gdt64_end:

; GDT descriptor
gdt64_descriptor:
    dw gdt64_end - gdt64_start ; Size
    dd gdt64_start             ; Start address 

; Segment Selector
CODE_SEG_64 equ gdt64_code - gdt64_start
DATA_SEG_64 equ gdt64_data - gdt64_start