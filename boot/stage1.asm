; Stage 1 of the bootloader
; Assembled File Size is not to surpass 512 Bytes
; Use LBA to Read directly from the Disk

[BITS 16]       ; CPU Executes the following Code in 16-Bit Modee
[ORG 0x7C00]    ; Starting Address assigned by the BIOS

start:
    cli
    xor ax, ax

    ; The three segment registers (Used by BIOS)
    mov ds, ax
    mov es, ax
    mov ss, ax
    
    ; Set the stack pointer
    mov sp, 0x7C00
    
    ; Clear Direction Flag and Set interrupt flag
    cld
    sti

    ; Store the BootDrive
    mov [boot_drive], dl ; Store the bootdrive ID into memory (BIOS Stores it into dl)

    ; Verify that LBA is supported
    xor cx, cx
    mov ah, 0x41
    mov bx, 0x55AA
    mov dl, [boot_drive]
    int 0x13

    ; If LBA is NOT supported: carry is set, BX will still be 0x55AA
    jc .no_lba
    cmp bx, 0xAA55
    jne .no_lba

    ; If the extended Functions are available CX has the 0th bit set to 1
    test cx, 1
    jz .no_lba
    
    ; Jump if supported
    jmp supported

.no_lba:
    mov si, lba_error
    jmp printerror

; Prints the error of what ever string is loaded into reg: si
printerror:
    lodsb
    test al, al ; Check for the NULL Byte
    jz hang
    call printchar
    jmp printerror

; Prints the ASCII value of what is in reg: al
printchar: 
    mov ah, 0x0E    ; Write Character in TTY Mode
    mov bh, 0x00    ; Page no
    mov bl, 0x07    ; Test attr 0x07 is lightgrey font on black background

    int 0x10        ; Video Interrupt BIOS Call
    ret

hang:
    hlt
    jmp hang


; LBA Is Supported
supported:
    ; Load the Stage 2 Bootloader
    mov si, dap
    mov word [dap + 2], STAGE2_SECTORS   ; number of sectors
    mov word [dap + 4], 0x8000           ; offset
    mov word [dap + 6], 0x0000           ; segment
    mov dword [dap + 8], 1               ; LBA low (start at 1)
    mov dword [dap + 12], 0              ; LBA high

    mov dl, [boot_drive]
    mov ah, 0x42                         ; INT 13h 42h extensions: Extended Read
    int 0x13
    jc .disk_fail

    ; jmp to stage 2 of the bootloader (0000:8000)
    jmp 0x0000:0x8000

.disk_fail:
    mov si, disk_error
    jmp printerror

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

lba_error db 'LBA Error',0
disk_error db 'Disk Error',0
boot_drive db 0

STAGE2_SECTORS equ 64

TIMES 510 - ($ - $$) db 0
dw 0xAA55