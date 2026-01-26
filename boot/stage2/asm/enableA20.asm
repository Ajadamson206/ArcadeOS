; File for enabling A20 Addressing: Needed for eventual 32-bit code execution
; NOTE: Most of this code is heavily inspired, if not directly from, the OSDevWiki Entry for A20
; https://wiki.osdev.org/A20_Line
[BITS 16]       ; CPU Executes the following Code in 16-Bit Modee

; This File Assumes that Interrupts are already disabled

; We are checking if memory locations that differ by 1MB refer to 
; different memory locations (A20 Enabled) or alias to the same memory (Disabled - wraparound)
check_a20:
    ; Save current state
    pushf
    push ds
    push es
    push di
    push si

    xor ax, ax
    mov es, ax ; ES Points to low memory 0x0000

    not ax
    mov ds, ax ; DS Points to high memory 0xFFFF (Just below 1MB)

    mov di, 0x0500 ; Low Address: [0x0000:0x0500] -> 0x000500
    mov si, 0x0510 ; High Address: [0xFFFF:0x0510] -> 0x100500

    ; Save Original Bytes that are in this location
    mov al, byte [es:di]
    push ax
    mov al, byte [ds:si]
    push ax

    ; Write Different values to the low and high addresses
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF

    ; A20 Disabled: 0x100500 aliases 0x000500 so the 0xFF overwrote the 0x00
    ; A20 Enabled: Nothing was overwritten so [es:di] == 0x00 still
    cmp byte [es:di], 0xFF

    ; Restore Original Stage
    pop ax
    mov byte [ds:si], al
    pop ax
    mov byte [es:di], al
    
    mov ax, 0

    ; Jump Condition from the comparison earlier (A20 Disabled)
    je check_a20__exit

    mov ax, 1

check_a20__exit:
    pop si
    pop di
    pop es
    pop ds
    popf

    ret

; Sets AX to 1 if it has succeeded, sets AX to 0 if it has failed
enable_a20:
    ; Test if A20 is already enabled - if it is you don't need to do anything at all
    call check_a20
    test ax, ax
    jnz already_enabled

    ; Try the BIOS function. Ignore the returned status.
bios_a20:
    mov ax, 0x2401 
    int 0x15 
    
    ; Test if A20 is enabled (to see if the BIOS function actually worked or not)
    call check_a20
    test ax, ax
    jnz already_enabled

    ; Try the keyboard controller method.
keyb_cont_a20:
    mov al, 0xdd  ;Look at the command list 
    out 0x64, al   ;Command Register 
    
    ; Test if A20 is enabled in a loop with a time-out (as the keyboard controller method may work slowly)
    call check_a20
    test ax, ax
    jnz already_enabled
    
    ; Try the Fast A20 method last
fast_a20:
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Test if A20 is enabled in a loop with a time-out (as the fast A20 method may work slowly)
    call check_a20
    test ax, ax
    jnz already_enabled
    
    ; If none of the above worked, give up
    xor ax, ax
    ret

already_enabled:
    mov ax, 1
    ret
