; This file defines the low-level ISR stubs for the first 32 CPU exceptions.

; External handlers from C kernel
[extern isr_handler]
[extern irq_handler] 


; --- Macros for saving and restoring state ---
%macro PUSHALL 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro POPALL 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

; --- Macro for ISRs that do NOT have an error code ---
; The CPU does not push an error code for these exceptions.
; We manually push a '0' to keep the stack consistent.
%macro ISR_NOERRCODE 1
  global isr_%1
  isr_%1:
    cli          ; Disable interrupts
    push qword 0 ; Push a dummy error code
    push qword %1; Push the interrupt number
    jmp isr_common_stub
%endmacro

; --- Macro for ISRs that DO have an error code ---
; The CPU automatically pushes an error code for these.
%macro ISR_ERRCODE 1
  global isr_%1
  isr_%1:
    cli          ; Disable interrupts
    push qword %1; Push the interrupt number
    jmp isr_common_stub
%endmacro

; --- Common stub that all ISRs will jump to ---
isr_common_stub:
    PUSHALL      ; Save all general purpose registers
    
    mov rdi, rsp ; The first argument to isr_handler is a pointer to the saved registers
    call isr_handler

    POPALL       ; Restore all registers
    
    add rsp, 16  ; Clean up the interrupt number and error code from the stack
    iretq        ; Return from interrupt
    sti          ; Re-enable interrupts


; System Call ISR macros
%macro ISR_SYSCALL 1
  global isr_%1
  isr_%1:
    cli
    push qword 0  ; Dummy error code
    push qword %1 ; Interrupt number (128)
    jmp isr_common_stub
%endmacro

; --- Generate the ISR stubs ---
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE   21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_ERRCODE   28
ISR_ERRCODE   29
ISR_ERRCODE   30
ISR_ERRCODE   31

; System call ISR stub
ISR_SYSCALL   128

irq_common_stub:
    PUSHALL

    mov rdi, rsp ; Pass a pointer to the saved registers to the C handler
    call irq_handler
    
    POPALL
    add rsp, 16  ; Clean up IRQ number and dummy error code
    sti
    iretq

; --- Macro for generating IRQ stubs ---
%macro IRQ 2
  global irq_%1
  irq_%1:
    cli
    push qword 0  ; Push a dummy error code
    push qword %2 ; Push the interrupt number (32-47)
    jmp irq_common_stub
%endmacro

; --- Generate all 16 IRQ stubs ---
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47