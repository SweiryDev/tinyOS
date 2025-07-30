[bits 32]

; Basic 4 level page table structure
setup_paging:
    ; 4 tables for 4 level paging, 
    ; each is 4KB (4096 Bytes)
    ; PML4T -> 0x1000
    ; PDPT  -> 0x2000
    ; PDT   -> 0x3000
    ; PT    -> 0x4000

    ; Zero out the memory of the page tables
    mov edi, 0x1000 ; Start address
    mov ecx, 4096   ; 4KB to write (counter)
    xor eax, eax    ; reset eax to 0
    rep stosd       ; repeat write until ecx=0
    ; stosd => store EAX at address EDI

    ; Create page table entries
    mov dword[PML4T], 0x2003 ; address of PDPT + Present and RW flags
    mov dword[PDPT], 0x3003 ; same for PDT
    mov dword[PDT], 0x4003  ; same for PT

    ; Fill the first Page Table to map 2MB
    mov edi, PT     ; PT is the first Page Table
    mov eax, 0b11 ; Present and RW flags
    mov ecx, 512    ; 512 entries in the table
    ; 4096 Bytes per table / 8 Bytes entry = 512 entries

; Loop until ecx=0
.fill_pt_loop:
    mov [edi], eax
    add eax, 0x1000 ; go to the next 4KB physical page
    add edi, 8      ; go to the next entry in the table
    loop .fill_pt_loop

    ret 


PML4T equ 0x1000
PDPT equ 0x2000
PDT equ 0x3000
PT  equ 0x4000

