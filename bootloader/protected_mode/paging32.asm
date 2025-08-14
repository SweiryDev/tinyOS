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

    ; Create page table entries (PML4 + PDPT + PDT + 6 * PT = 9 tables = 36kb)
    mov dword[PML4T], 0x11003 ; address of PDPT + Present and RW flags
    mov dword[PDPT], 0x12003 ; same for PDT
    mov dword[PDT], 0x13003  ; same for PT
    mov dword[PDT+8], 0x14003 
    mov dword[PDT+16], 0x15003
    mov dword[PDT+24], 0x16003
    mov dword[PDT+32], 0x17003
    mov dword[PDT+40], 0x18003


    ; Fill the first Page Table to map 12MB
    mov edi, PT     ; PT is the first Page Table
    mov eax, 0b11   ; Present and RW flags
    mov ecx, 512*6  ; 512 entries in the table (6 tables)
    ; 4096 Bytes per table / 8 Bytes entry = 512 entries

; Loop until ecx=0
.fill_pt_loop:
    mov [edi], eax
    add eax, 0x1000 ; go to the next 4KB physical page
    add edi, 8      ; go to the next entry in the table
    loop .fill_pt_loop

    ret 


PML4T equ 0x10000
PDPT equ 0x11000
PDT equ 0x12000
PT  equ 0x13000

