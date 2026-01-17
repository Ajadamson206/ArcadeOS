[BITS 16]

get_map:
    pushf
    push ds
    push es
    push di
    push si
    
    ; Location 0x009B00 is going to be used to keep track of the number of entries
    xor ax, ax
    mov ds, ax

    mov si, 0x9B00
    mov word [si], 0
    
    ; We are going to write the memory map at 0x009C00
    mov es, ax
    mov di, 0x9C00

    ; First Call:
    ; point ES:DI at the destination buffer for the list. Clear EBX. 
    ; Set EDX to the magic number 0x534D4150. Set EAX to 0xE820
    ; Set ECX to 24. Do an INT 0x15.
    mov eax, 0xE820
    xor ebx, ebx
    mov ecx, 24
    mov edx, 0x534D4150
    int 0x15

    ; If it was not successful
    jc .error

    inc byte [si] 

    ; If ECX = 20 add 4 more bytes for padding (Preserve the 24 Byte Quantity)
    xor ecx, 20
    test cx, cx
    jnz .no_padding
    mov dword [si + 20], 0
.no_padding:
    ; If bx = 0 then we reached the end of the list
    test bx, bx
    jnz .success
    jmp .safe_exit
    
.success:
    ; Continue until bx = 0

    ; Increment di by 24
    mov cx, 24
    add di, cx
    
    ; Reset BIOS Interrupt
    mov eax, 0xE820
    mov ecx, 24
    int 0x15

    jc .error

    inc byte [ds:si] ; Increment the number of entries

    ; If bx = 0 then we reached the end of the list
    test bx, bx
    jnz .success
.safe_exit:
    pop si
    pop di
    pop es
    pop ds
    popf
    ret
.error:
    mov si, memory_map_err
    call printstring

print_mem_map_entry:
    push ax
    xor ax, ax
    mov al, cl
    call printint
    pop ax

    mov si, new_line
    call printstring




memory_map_err db 'Failed to get the memory map',13,10,0
hex_start db '0x',0