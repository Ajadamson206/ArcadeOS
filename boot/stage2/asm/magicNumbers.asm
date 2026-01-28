[BITS 16]
global boot_drive
global lower_mem_size

; Misc Variables stored in memory
boot_drive:
    db 0
lower_mem_size:
    dw 0
hex_conv db '0123456789ABCDEF'  ; Map for converting from binary -> ASCII Hex
KERNEL_BYTES equ 10000          ; Size of Kernel in Bytes
KERNEL_SECTORS equ 12              ; Number of Sectors the Kernel uses (ceil[number of bytes / 512]) 