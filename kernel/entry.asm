[bits 64]
[extern main]

section .bss
align 16
stack_bottom:
    resb 4096 * 4   ; Reserve 16KB for the stack
stack_top:

section .text
global kernel_start
kernel_start:
    mov rsp, stack_top
    call main
    jmp $
