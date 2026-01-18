[BITS 16]

; We can assume that LBA is already supported
loadkernel:
    ; Load the Kernel into memory
    ; We are staging the kernel, so we store it in a temporary place (0x00200000)
    ; The Bootloader's C Code will parse the required information
    mov si, dap
    mov word [dap + 2], KERNEL_SECTORS      ; number of sectors (ceil[numBytes / 512])
    
    mov ah, 0x0002
    mov al, 0x0000
    mov word [dap + 4], ax               ; offset
    mov word [dap + 6], 0x0000           ; segment
    
    ; This portion is which disk sector the beginning of the kernel is (ByteOffset / 512)
    mov dword [dap + 8], 64              ; LBA low (start at 1)
    mov dword [dap + 12], 0              ; LBA high

    mov dl, [boot_drive]
    mov ah, 0x42                         ; INT 13h 42h extensions: Extended Read
    int 0x13
    jc .disk_fail

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
