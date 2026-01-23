[BITS 32]

pmode_entry:
    ; set segment registers, set esp (32-bit stack pointer), cld
    xor eax, eax

    mov ax, 0x10        ; Data Segment Part of GDT 
    mov ds, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax 
    mov ss, ax

    mov esp, 0x0090000
    cld

    ; Call stage2_main (32-bit C code, but still in bootloader process)
