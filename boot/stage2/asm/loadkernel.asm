[BITS 16]

KERNEL_LBA equ 12

BOUNCE_SEG equ 0x0A00
BOUNCE_OFF equ 0x0000

; We can assume that LBA is already supported and we are in Unreal Mode
loadkernel:
    clc
    cld

    push cs
    pop ds

; Load the Kernel into memory
; We read a sector, store it at 0x0000A000 then copy it to
; 0x00100000
; The Bootloader's C Code will parse the required information
.load_sector:

    mov si, dap
    mov word [dap + 2], KERNEL_SECTORS   ; Load one sector at a time
    
    ; Where to write
    mov word [dap+4], BOUNCE_OFF        ; offset
    mov word [dap+6], BOUNCE_SEG        ; segment
    
    ; This portion is which disk sector the current part of the kernel is
    mov dword [dap + 8], KERNEL_LBA     ; LBA low
    mov dword [dap + 12], 0             ; LBA high

    xor ax, ax
    mov dl, [boot_drive]
    mov ah, 0x42                         ; INT 13h 42h extensions: Extended Read
    int 0x13
    jc .disk_fail
    
.check_elf:
    push ds
    
    mov ax, BOUNCE_SEG
    mov ds, ax
    mov bx, BOUNCE_OFF
    mov eax, [bx]

    pop ds

    ; Compare against ELF magic
    cmp eax, 0x464C457F      ; 0x7F 'E' 'L' 'F' (little-endian)
    jne .not_elf

    mov si, kernel_elf
    call printstring
    jmp .done

.not_elf:
    mov si, kernel_elf_fail
    call printstring

.done:
    ret

.disk_fail:
    mov si, kernel_error
    call printstring
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
