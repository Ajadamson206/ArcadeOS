; More Complex than Stage 1, Needs to...
; 1. real-mode init + print
; 2. disk read routine (INT13h extensions)
; 3. enable A20
; 4. read kernel from fixed LBA into 0x00100000
; 5. get E820 map
; 6. set up GDT
; 7. enter protected mode
; 8. jump to kernel entry, pass boot info ptr

[BITS 16]       ; CPU Executes the following Code in 16-Bit Modee
[ORG 0x8000]

jmp stage2

; Include Files
%include "boot/stage2/enableA20.asm"
%include "boot/stage2/e820map.asm"

; We need to treat this as a completely new file than Stage 1
; Only assumption we can make is LBA is compatible
stage2:
    cli
    xor ax, ax

    ; The three segment registers (Used by BIOS)
    mov ds, ax
    mov es, ax
    mov ss, ax
    
    ; Set the stack pointer
    mov sp, 0x9000 ; Stack Pointer needs to change since we moved memory locations
    cld    

    ; Store the BootDrive
    mov [boot_drive], dl ; Store the bootdrive ID into memory (BIOS Stores it into dl)
    mov al, dl
    call printint    

    mov si, new_line
    call printstring

    ; Move the string to si and print it to screen
    mov si, stage2_string
    call printstring

    ; Enable A20
    call enable_a20
    test ax, ax ; Will Set the ZF flag if it failed
    jz a20failure
    mov si, a20good
    call printstring

    ; Get E820 Map
    call get_map
    
    push ds
    push si

    mov si, 0x9B00
    mov ax, word [si]

    call printint

    pop si
    pop ds

    ; Load Kernel (INT 13h)

    ; Build boot-info struct

    ; Load Global Descriptor Table, set cr0.pe, far jump into 32-bit

    ; set segment registers, set esp (32-bit stack pointer), cld

    ; Call stage2_main (32-bit C code, but still in bootloader process)


hang:
    hlt
    jmp hang

; Prints what ever string is loaded into reg: si
printstring:
    push ax
.str_loop: 
    lodsb
    test al, al ; Check for the NULL Byte (Sets zero flag if null)
    jz .done
    call printchar
    jmp .str_loop
.done:
    pop ax
    ret

; Prints the ASCII value of what is in reg: al
printchar:
    push ax
    push bx
    push cx
    push dx

    mov ah, 0x0E    ; Write Character in TTY Mode
    mov bh, 0x00    ; Page no
    mov bl, 0x07    ; Test attr 0x07 is lightgrey font on black background

    int 0x10        ; Video Interrupt BIOS Call

    pop dx
    pop cx
    pop bx
    pop ax
    ret

; Prints an ASCII String of the value in ax
printint:
    ; Push registers onto the stack
    push ax
    push bx
    push cx
    push dx

    xor bx, bx
    mov cx, 10

; Divide the numbers and push them onto the stack
.printint_loop:
    xor dx, dx
    div cx ; AX := Quotient, DX := Remainder

    add dl, '0' ; Convert Digit to ascii
    push dx
    inc bl
    
    test ax, ax
    jnz .printint_loop

; Pop numbers off the stack and print them
.printint_pop:
    pop dx
    mov al, dl
    push bx
    call printchar
    pop bx
    dec bl

    test bl, bl
    jnz .printint_pop

    ; Restore Original Registers
    pop dx
    pop cx
    pop bx
    pop ax
    ret

; Temp Function
print_map:
    ; 0x009B00 has the number of entries
    ; 0x009C00 is where the entries start
    ; Each entry is 24 Bytes

    pushf
    push ds
    push es
    push di
    push si

    ; Store number of entries into cx
    xor ax, ax
    mov ds, ax

    mov si, 0x9B00
    mov cx, word [si]
    
    ; We are going to write the memory map at 0x009C00
    mov es, ax
    mov di, 0x9C00

a20failure:
    mov si, a20fail
    call printstring
    jmp hang 

; Error Messages
a20fail db 'Enabling A20 has failed.',13,10,0

; General Messages
stage2_string db 'Entering Stage 2:',13,10,0
a20good db 'Successfully enable A20',13,10,0
new_line db 13,10,0
low_memory db 'Lower Memory Size: ',0
high_memory db 'Higher Memory Size: ',0

; Misc Variables stored in memory
boot_drive db 0
kernel_size dw 0 ; Size of the Kernel in Bytes

TIMES 1024 - ($ - $$) db 0
