[BITS 16]

buildbootinfo:
    push si

    ; Read the number of entries in the E820 Map
    mov si, 0x9B00
    mov cx, word [si]

    ; Init the struct
    mov si, bootinfo_struct

    ; 2nd 8-Byte Chunk
    mov byte [bootinfo_struct + 8], [boot_drive]    
    mov eax, 0x0000000000009C00
    mov dword [bootinfo_struct + 9], eax
    mov byte [bootinfo_struct + 13], cx

    ; 3rd 8-Byte Chunk
    mov eax, 0x0000000000100000
    mov dword [bootinfo_struct + 16], eax
    mov dword [bootinfo_struct + 20], KERNEL_BYTES

    ; 4th 8-Byte Chunk
    mov eax, 0x0000000000008000
    mov dword [bootinfo_struct + 24], eax
    mov dword [bootinfo_struct + 28], 0x600     ; This will need to be updated when stage 2 grows due to the C Code

    ; Ignore the Binary Flags for now

    ; Exit
    pop si
    ret


bootinfo_struct:
    dd 0xFF12FF12FF12FF12   ; A Unique Value that says that this is my bootloader
    dw 0x1                  ; Version info (Version 1)
    dw 0x28                 ; Size of the struct in bytes (40 Bytes)

    db 0                    ; Boot drive id (DL Register)
    dd 0x                   ; Pointer to the e820 map: 0x009C00
    dw 0                    ; Number of entries in the e820 map: 0x009B00
    db 0                    ; Padding

    dd 0                    ; Where the kernel starts (In memory)
    dd 0                    ; Size of Kernel (In Bytes)

    dd 0                    ; Where the stage 2 loader is in memory
    dd 0                    ; Size of stage 2 loader in bytes

    dq 0                    ; Misc Binary Flags that I may use