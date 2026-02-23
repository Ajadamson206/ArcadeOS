[BITS 16]

unreal_mode:
    cli

    ; Save the current real-mode CS
    mov ax, cs
    mov [rm_cs], ax

    ; Load gdt
    lgdt [cs:gdt_descriptor]

    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Jump into protected mode
    jmp 0x08:pmode         ; far jump flush

[BITS 32]
pmode:
    ; load big data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Back to real mode
    mov eax, cr0
    and eax, 0xFFFFFFFE
    mov cr0, eax

    ; Build the real-mode far pointer: rm_cs:rmode
    mov ax, [rm_cs]
    mov [rmode_ptr + 2], ax

    ; IMPORTANT: force 16-bit far jump (16:16) from 32-bit code
    o16 jmp far [rmode_ptr]

[BITS 16]
rmode:
    sti

    ; Write "OK" to VGA memory using ES to be unambiguous
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:0], 0x2F4F    ; 'O'
    mov word [es:2], 0x1F4B    ; 'K'
    pop es

    jmp loadkernel             ; now this will resolve correctly again

rm_cs     dw 0
rmode_ptr dw rmode, 0           ; dw offset, dw segment (filled at runtime)
