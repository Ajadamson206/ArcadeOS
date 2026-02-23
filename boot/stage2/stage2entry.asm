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

global stage2

jmp stage2      ; Include basically copy and pastes the files
                ; Jump is here so we go straight to stage2 header

; Include Files
%include "stage2/asm/magicNumbers.asm"

%include "stage2/asm/enableA20.asm"
%include "stage2/asm/e820map.asm"
%include "stage2/asm/buildbootinfo.asm"
%include "stage2/asm/unreal.asm"
%include "stage2/asm/loadkernel.asm"
%include "stage2/asm/vbepoll.asm"


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

    ; Get VBE Info
    ;call enable_graphics

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

    mov si, new_line
    call printstring

    pop si
    pop ds

    mov si, map_string
    call printstring

    call print_map

    ; Build boot-info struct
    call buildbootinfo

    mov si, test1
    call printstring

    ; Enter Unreal Mode
    jmp unreal_mode

.unreal_fin:
    ; Load the Kernel
    jmp 0x0:loadkernel

    jmp dword 0x08:pmode_entry    ; 0x08 Is the Code Segment of the GDT

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

; Print the hex of the number stored in al
printhex:
    push cx
    push si

    mov cx, 16
    div cl

    mov si, hex_conv

    ; al has the quotient, ah has the remainder    
    mov cl, ah
    add si, cx ; Increment pointer by hex number 0-15
    mov cl, al
    mov al, [si] ; Dereference the pointer to get the hex number
    xor ah, ah

    push ax
    mov si, hex_conv

    ; Cl currently has the quotient
    add si, cx
    mov al, [si]

    call printchar

    pop ax

    call printchar

    pop si
    pop cx
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
    
    ; We are going to read the memory map at 0x009C00
    mov es, ax
    mov di, 0x9C00

.map_loop:
    ; Test if there is data written there
    test cx, cx
    jz .map_exit

    ; 24 Bytes of data 8: Base Address 8: Region Length 4: Region Type 4: ACPI 3.0 Extended Attributes

    mov si, hex_string
    call printstring

    ; Print Address

    mov al, byte [di + 7]
    call printhex
    mov al, byte [di + 6]
    call printhex
    mov al, byte [di + 5]
    call printhex
    mov al, byte [di + 4]
    call printhex
    mov al, byte [di + 3]
    call printhex
    mov al, byte [di + 2]
    call printhex
    mov al, byte [di + 1]
    call printhex
    mov al, byte [di]
    call printhex


    mov al, 32
    call printchar

    ; Print Length
    mov si, hex_string
    call printstring

    mov al, byte [di + 15]
    call printhex
    mov al, byte [di + 14]
    call printhex
    mov al, byte [di + 13]
    call printhex
    mov al, byte [di + 12]
    call printhex
    mov al, byte [di + 11]
    call printhex
    mov al, byte [di + 10]
    call printhex
    mov al, byte [di + 9]
    call printhex
    mov al, byte [di + 8]
    call printhex

    mov al, 32
    call printchar

    ; Region Type
    mov si, hex_string
    call printstring

    mov al, byte [di + 19]
    call printhex
    mov al, byte [di + 18]
    call printhex
    mov al, byte [di + 17]
    call printhex
    mov al, byte [di + 16]
    call printhex

    mov al, 32
    call printchar

    ; APCI Extended
    mov si, hex_string
    call printstring

    mov al, byte [di + 23]
    call printhex
    mov al, byte [di + 22]
    call printhex
    mov al, byte [di + 21]
    call printhex
    mov al, byte [di + 20]
    call printhex

    mov si, new_line
    call printstring

    ; Decrement Counter, Increment DI
    dec cx
    add di, 24
    jmp .map_loop
    
.map_exit:
    pop si
    pop di
    pop es
    pop ds
    popf

    ret

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
hex_string db '0x',0
map_string db 'Memory Map: Base Address | Length | Type | ACPI 3.0 Attributes',13,10,0
test1 db "Unreal Precall",13,10,0
test2 db "Entered Unreal Mode",13,10,0


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

%include "stage2/asm/protectedmode.asm"