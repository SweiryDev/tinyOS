[bits 64]
[extern main]

section .text
global kernel_start
kernel_start:
    call main
    jmp $