[BITS 32]

; C Code Interrupt Handler
extern interrupt_dispatch
extern interrupt_keyboard_handler

;
; The IDT cannot directly call C functions, I need asm stubs
; which are used to call the C functions instead
;
; Macros are used so I don't have to copy and paste the same code over and over
; for each entry that I need.
;


; This macro is for functions that do not push their own error codes onto the stack
; Push the error code for them instead.
%macro ISR_NOERR 1
global isr%1
isr%1:
    push dword 0
    push dword %1
    jmp isr_common
%endmacro

; For now these are the suggested interrupts, more can be added later, 
; but it doesn't matter too much

; IDT_DIV_ZERO
ISR_NOERR 0

; IDT_INVALID_OPCODE
ISR_NOERR 6

; IDT_TIMER
ISR_NOERR 32

; IDT_KEYBOARD
ISR_NOERR 33

; This macro is for functions that do push their own error codes
%macro ISR_ERR 1
global isr%1
isr%1:
    push dword %1
    jmp isr_common
%endmacro

; IDT_DOUBLE_FAULT
ISR_ERR 8

; IDT_STACK_FAULT
ISR_ERR 12 

; IDT_GENERAL_PROTECTIOn
ISR_ERR 13

; IDT_PAGE_FAULT
ISR_ERR 14

isr_common:

    pusha

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call interrupt_dispatch
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds

    popa

    add esp, 8
    iret