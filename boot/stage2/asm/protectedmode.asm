[BITS 32]
section .text
global pmode_entry
global c_code_exit
global back_to_c

extern stage2_c_main
extern loadkernel

c_stack:
    dd 0

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
    cli
    
    call stage2_c_main

; Needs
back_to_c:
    cli

    ; set segment registers, set esp (32-bit stack pointer), cld
    xor eax, eax

    mov ax, 0x10        ; Data Segment Part of GDT 
    mov ds, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax 
    mov ss, ax

    mov esp, dword [c_stack]

    ret

.hang:
    hlt
    jmp .hang

c_code_exit:
    ; Save the stack pointer so we can go back to it later
    mov dword [c_stack], esp

    ; Leave Protected Mode
    cli

    ; Disable cr0
    mov eax, cr0
    and eax, 0x7FFFFFFE
    mov cr0, eax

    jmp 0x0000:real_mode_entry

[BITS 16]
protected_mode_enable:
    mov eax, cr0
    or eax, 1        ; Set the protection enable bit
    mov cr0, eax

    ; Prevent Crashes with XMM Instructions
    mov eax, cr0
    and eax, ~(1 << 2)     ; clear EM
    or  eax,  (1 << 1)     ; set MP
    mov cr0, eax

    mov eax, cr4
    or  eax, (1 << 9)      ; OSFXSR
    or  eax, (1 << 10)     ; OSXMMEXCPT (optional but recommended)
    mov cr4, eax    

    ret

real_mode_entry:

    ; Reset Segments
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    sti

    ; Go to loadkernel
    jmp loadkernel