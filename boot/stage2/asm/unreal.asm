[BITS 16]

unreal_mode:
    cli
    push ds
    push cs

    ; Load Global Descriptor Table
    lgdt [cs:gdt_descriptor]

    ; Enter Protected Mode
    mov eax, cr0
    or eax, 1           ; Set the protection enable bit
    mov cr0, eax    

    ; Flush prefetch queue
    jmp 0x08:pmode

[BITS 32]
pmode:
   mov  bx, 0x10          ; select descriptor 2
   mov  ds, bx            ; 10h = 10000b

   and al,0xFE            ; back to realmode
   mov  cr0, eax          ; by toggling bit again
   jmp 0x0:rmode

rmode:
    sti

    pop cs
    pop ds

    mov word [0xB8000], 0x1F4F   ; 'O'
    mov word [0xB8002], 0x1F4B   ; 'K'

    ret

rm_cs     dw 0
rmode_ptr dw rmode, 0           ; dw offset, dw segment (filled at runtime)

; Bits  0–15   : Limit (0–15)
; Bits 16–31   : Base  (0–15)
; Bits 32–39   : Base  (16–23)
; Bits 40–47   : Access Byte
; Bits 48–51   : Limit (16–19)
; Bits 52–55   : Flags
; Bits 56–63   : Base  (24–31)

; I am using a simplified model (Ring 0 & No Memory Protection) so only these entries are required
; Null Descriptor (All Zeros)
; Kernel Mode Code Segment
; Kernel Mode Data Segment

align 8
gdt_start:
    dq 0x0000000000000000   ; Null Descriptor (All Zeros)
    
    dq 0x00CF9A000000FFFF   ; Kernel Mode Code Segment

    dq 0x00CF92000000FFFF   ; Kernel Mode Data Segment
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start