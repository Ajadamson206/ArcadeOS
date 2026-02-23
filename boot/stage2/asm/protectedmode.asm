[BITS 32]
section .text
global pmode_entry
extern stage2_c_main

pmode_entry:
    cli

    ; set segment registers, set esp (32-bit stack pointer), cld
    xor eax, eax

    mov ax, 0x10        ; Data Segment Part of GDT 
    mov ds, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax 
    mov ss, ax

    ; fuck it move the stack to a weird spot (We will change it in the kernel)
    mov esp, 0x01000000
    and esp, 0xFFFFFFF0
    cld

    mov ecx, 0x1F4B1F4F
    mov dword [0xB8000], 0x1F4B1F4F  ; OK

    ; Call stage2_c_main (32-bit C code, but still in bootloader process)
    call stage2_c_main

.hang:
    hlt
    jmp .hang
