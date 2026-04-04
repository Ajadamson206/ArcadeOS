[BITS 16]

BYTES_PER_SECTOR equ 128 ; Divide by 512/4 since we are using the movsd instr

secs_to_move:
    dw 0

; Enter Protected Mode then jump to 32-bit
; Assume that ax has the number of sectors to move
movekernel_init:
    cli

    mov word [secs_to_move], ax

    lgdt [gdt_descriptor]

    ; Enter Protected Mode then jump to movekernel
    mov eax, cr0
    or eax, 1        ; Set the protection enable bit
    mov cr0, eax

    ; Prevent Crashes with XMM Instructions
    mov eax, cr0
    and eax, ~(1 << 2)     ; clear EM
    or  eax,  (1 << 1)     ; set MP
    mov cr0, eax

    jmp dword 0x08:movekernel 

[BITS 32]
kernel_address:
    dd 0x00300000 ; Move it to here so we can then copy what we need to 0x00200000

; Assumes that the kernel was first loaded at 0xA000
movekernel:
    cli

    ; Set segment registers, set esp (32-bit stack pointer), cld
    xor eax, eax

    mov ax, 0x10        ; Data Segment Part of GDT 
    mov ds, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax 
    mov ss, ax

    cld

    ; Move the kernel
    mov esi, 0x0000A000
    mov edi, dword [kernel_address]
    xor eax, eax
    mov ax, [secs_to_move]
    mov ecx, BYTES_PER_SECTOR
    mul ecx
    mov ecx, eax

    rep movsd
    
    ; EDI is incremented each iteration
    mov dword [kernel_address], edi

    jmp 0x18:preload16_exit

.hang:
    hlt
    jmp .hang

[BITS 16]
preload16_exit:
    mov ax, 0x20
    mov ds, ax
    mov es, ax
    mov fs, ax 
    mov gs, ax 
    mov ss, ax

    mov eax, cr0
    and eax, 0xFFFFFFFE
    mov cr0, eax

    jmp 0x0000:preload_real

preload_real:
    cli

    ; Reset Segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov sp, 0x7C00

    jmp load_sector
