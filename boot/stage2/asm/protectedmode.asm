[BITS 32]
section .text
global pmode_entry
extern stage2_c_main

pmode_entry:
    cli

    ; set segment registers, set esp (32-bit stack pointer), cld
    xor eax, eax

    ; Enter protected mode (CR0.PE = 1)
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Update Stack
    mov esp, 0x0009F000
    and esp, 0xFFFFFFF0
    cld

    ; Call stage2_c_main (32-bit C code, but still in bootloader process)
    call stage2_c_main

.hang:
    hlt
    jmp .hang

