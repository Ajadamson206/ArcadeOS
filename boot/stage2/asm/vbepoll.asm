[BITS 16]

; Code to Poll information from the BIOS
; Sets up the Linear Frame Buffer for writing pixels to the screen

; Set everything to 640 x 480 with 32 bpp (RGBX)

enable_graphics:

    ; Get VESA BIOS Information:
    ; BIOS Interrupt Int 0x10 AX=0x4F00
    ; Input: ES:DI - Segment:Offset Pointer where BIOS info struct is
    ; Outputs AX = 0x004F if succeeded

    mov byte [vbe_info_signature], 'V'
    mov byte [vbe_info_signature + 1], 'E'
    mov byte [vbe_info_signature + 2], 'S'
    mov byte [vbe_info_signature + 3], 'A'

    push es
    mov ax, 0x4F00
    mov di, vbe_info_struct
    int 0x10

    pop es

    ; Check if it was a success
    cmp ax, 0x004F
    jne .vbe_fail

    ; Store the segment offset + segment
    mov ax, word [video_modes_segment]
    mov ds, ax

    mov ax, word [video_modes_offset]
    mov si, ax

.verify_mode:
    mov dx, [ds:si]
    add si, 2   ; Increment to next mode

    cmp dx, 0xFFFF ; Check if we reached the end of the array
    je .vbe_fail

    ; Get VESA mode Information:
    ; BIOS Intr Int AX=0x4F01
    ; Input: CX = VESA mode number from video modes array
    ; Input: ES:DI - Segment:Offset pointer where to store the struct
    ; Outputs AX = 0x004F on success

    push es
    push dx
    mov ax, 0x4F01
    mov cx, dx
    mov di, vbe_mode_info_struct
    int 0x10
    pop dx
    pop es

    cmp ax, 0x4F
    jne .vbe_fail

    ; Verify that info matches desired (640 width, 480 height, 32 bpp)
    ; If it doesn't match continue loop

    ; Width (640 -> 0x0280)
    mov ax, 0x0280    
    cmp ax, [vbe_mode_width] 
    jne .verify_mode

    ; Height (480 -> 0x01E0)
    mov ax, 0x01E0
    cmp ax, [vbe_mode_height] 
    jne .verify_mode

    ; Bits Per Pixel
    xor ax, ax
    mov al, 0x20
    cmp al, [vbe_mode_bpp] 
    jne .verify_mode

    ; Assuming we have the right mode info:
 
    ; Set VBE mode
    ; Bios Interrupt Int 0x10 AX=4F02
    ; Input BX = Bits 0-13 mode number; bit 14 enables linear framebuffer;
    ;            Bit 15 0 - Clear the frame buffer, 1 - Leave video memory untouched
    ; Output AX=0x004F on success
    mov ax, 0x4F02
    mov bx, dx
    or bx, 0x2000
    int 0x10

    cmp ax, 0x4F
    jne .vbe_fail

    mov si, vbe_success
    call printstring

    cli

    ret

.vbe_fail:
    mov si, vbe_error
    call printstring
    jmp hang




vbe_error db "Failed To Set VBE Mode to 640 x 480: 32 bpp",13,10,0
vbe_success db "Successfully set to 640 x 480: 32 bpp",13,10,0


