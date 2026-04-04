[BITS 16]

kernel_lba:
    dd 13

BOUNCE_SEG equ 0x0A00
BOUNCE_OFF equ 0x0000

MAX_LOAD_AMOUNT equ 60

times_to_load_full:
    dw 0

remaining_sectors:
    dw 0

; We can assume that LBA is already supported and we are in Unreal Mode
loadkernel:
    clc
    cld

    mov si, kernel_start
    call printstring

    push cs
    pop ds

    ; Determine the number of times needed to load the full 60 sectors
    ; times = (KERNEL_SECTORS) / 60
    xor edx, edx

    mov eax, KERNEL_SECTORS
    mov ecx, MAX_LOAD_AMOUNT
    div ecx ; Unsigned divide EDX:EAX by ECX

    mov word [times_to_load_full], ax ; Quotient
    mov word [remaining_sectors], dx ; Remainder

; Load the Kernel into memory
; We read a sector, store it at 0x0000A000 then copy it to
; 0x00100000
; The Bootloader's C Code will parse the required information
load_sector:
    mov ax, word [times_to_load_full]
    test ax, ax ; Check if zero
    jz .load_remainder

    mov si, dap
    mov word [dap + 2], MAX_LOAD_AMOUNT   ; Load the max sectors
    
    ; Where to write
    mov word [dap+4], BOUNCE_OFF        ; offset
    mov word [dap+6], BOUNCE_SEG        ; segment
    
    ; This portion is which disk sector the current part of the kernel is
    mov eax, dword [kernel_lba]
    mov dword [dap + 8], eax            ; LBA low
    mov dword [dap + 12], 0             ; LBA high

    xor ax, ax
    mov dl, [boot_drive]
    mov ah, 0x42                         ; INT 13h 42h extensions: Extended Read
    int 0x13
    jc .disk_fail

    ; Update the LBA
    mov eax, dword [kernel_lba]
    add eax, MAX_LOAD_AMOUNT
    mov dword [kernel_lba], eax

    ; Decrement the times to load full
    mov ax, word [times_to_load_full]
    dec ax
    mov word [times_to_load_full], ax

    ; Move the kernel to the correct spot
    mov ax, MAX_LOAD_AMOUNT
    jmp movekernel_init

.load_remainder:
    mov ax, word [remaining_sectors]
    test ax, ax
    jz .done

    mov si, dap
    mov word [dap + 2], ax              ; Load remaining sectors
    
    ; Where to write
    mov word [dap+4], BOUNCE_OFF        ; offset
    mov word [dap+6], BOUNCE_SEG        ; segment
    
    ; This portion is which disk sector the current part of the kernel is
    mov eax, dword [kernel_lba]
    mov dword [dap + 8], eax            ; LBA low
    mov dword [dap + 12], 0             ; LBA high

    xor ax, ax
    mov dl, [boot_drive]
    mov ah, 0x42                         ; INT 13h 42h extensions: Extended Read
    int 0x13
    jc .disk_fail

    ; Set Remaining Sectors to 0
    mov ax, [remaining_sectors]
    mov cx, 0
    mov word [remaining_sectors], cx
    jmp movekernel_init

.done:
    cli

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

    jmp dword 0x08:back_to_c
    

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
kernel_start db 'Loading the Kernel',13,10,0