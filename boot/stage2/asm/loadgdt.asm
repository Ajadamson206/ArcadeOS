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

[BITS 16]

align 8
gdt_start:
    dq 0x0000000000000000   ; Null Descriptor (All Zeros)
    
    dq 0x00CF9A000000FFFF   ; Kernel Mode Code Segment

    dq 0x00CF92000000FFFF   ; Kernel Mode Data Segment

    dq 0x00009A000000FFFF   ; 0x18 16-bit code
    dq 0x000092000000FFFF   ; 0x20 16-bit data
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start