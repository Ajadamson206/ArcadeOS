[BITS 16]

KERNEL_LBA equ 12

BOUNCE_SEG equ 0x1000
BOUNCE_OFF equ 0x0000

BOUNCE_END equ 0x00010200
BOUNCE_STR equ 0x00010000

sec_loaded dw 0

load_dst dd 0x00100000
load_src dd 0

; We can assume that LBA is already supported and we are in Unreal Mode
loadkernel:
    clc
    cld

    push cs
    pop ds

; Load the Kernel into memory
; We read a sector, store it at 0x00010000 then copy it to
; 0x00100000
; The Bootloader's C Code will parse the required information
.load_sector:

    mov si, dap
    mov word [dap + 2], 1       ; Load one sector at a time
    
    ; Where to write
    mov word [dap+4], BOUNCE_OFF        ; offset
    mov word [dap+6], BOUNCE_SEG        ; segment
    
    ; This portion is which disk sector the current part of the kernel is
    ; LBA = KERNEL_LBA + sec_loaded
    xor eax, eax
    mov ax, [sec_loaded]
    add eax, KERNEL_LBA
    mov dword [dap + 8], eax     ; LBA low (start at 1)
    mov dword [dap + 12], 0      ; LBA high

    xor ax, ax
    mov dl, [boot_drive]
    mov ah, 0x42                         ; INT 13h 42h extensions: Extended Read
    int 0x13
    jc .disk_fail

; Copy Sector to 0x00100000 + prev copies
    mov eax, BOUNCE_STR
    mov [load_src], eax

    push ds
    push es
    push si
    push di

; Copy sector with rep movsd instruction
    mov esi, BOUNCE_STR
    mov edi, [load_dst]
    
    ; Copy 512 Bytes (128 Dwords)
    mov ecx, 128

    ; Move (E)CX doublewords from DS:[(E)SI] to ES:[(E)DI].
    rep movsd

    ; Increment 
    mov eax, [load_dst]
    add eax, 512
    mov [load_dst], eax

; Copy End
    pop di
    pop si
    pop es
    pop ds

    xor eax, eax
    inc [sec_loaded]

    mov ax, [sec_loaded]
    cmp ax, KERNEL_SECTORS
    jb .load_sector  ; sec_loaded < KERNEL_SECTORS
    
.check_elf:
    push ds
    
    mov ax, [BOUNCE_SEG]
    mov ds, ax
    mov bx, [BOUNCE_OFF]
    mov eax, [bx]

    pop ds

    ; Compare against ELF magic
    cmp eax, 0x464C457F      ; 0x7F 'E' 'L' 'F' (little-endian)
    jne .not_elf

    mov ecx, 0x1F4B1F4F
    mov dword [0xB8000], ecx
    jmp .done

.not_elf:
    mov ecx, 0x1F4B1F4F
    mov dword [0xB8000], ecx

.done:
    jmp dword 0x08:pmode_entry    ; 0x08 Is the Code Segment of the GDT

.disk_fail:
    mov ecx, 0x1F4B1F4F
    mov dword [0xB8000], ecx
    jmp hang

; Disk Address Packet (DAP)
;  0	1	size of packet (16 bytes)
;  1	1	always 0
;  2	2	number of sectors to transfer (max 127 on some BIOSes)
;  4    2   transfer buffer 16 bit offset
;  6	2	transfer buffer 16 bit segment
;  8	4	lower 32-bits of 48-bit starting LBA
; 12	4	upper 16-bits of 48-bit starting LBA
dap:
    db 0x10
    db 0
    dw 0
    dw 0
    dw 0
    dd 0
    dd 0

kernel_error db 'Error Reading the Kernel into Memory',13,10,0
kernel_elf db 'Successfully loaded ELF Kernel',13,10,0
kernel_elf_fail db 'Unable to find ELF Header',13,10,0
