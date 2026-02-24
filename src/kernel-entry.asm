; Entry Stub for the Kernel
; Needed to convert the EAX and EBX to parameters
; for the kernel_main function call

[BITS 32]

global _kernel_start
extern kernel_main

_kernel_start:
    ; EAX = multiboot magic
    ; EBX = multiboot info pointer

    push ebx
    push eax
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang