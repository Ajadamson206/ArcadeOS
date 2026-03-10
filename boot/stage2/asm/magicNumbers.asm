[BITS 16]
global boot_drive
global lower_mem_size
global vbe_info_struct
global vbe_mode_info_struct
global vbe_current_mode
global KERNEL_SECTORS
global mem_map_entries

section .bss

mem_map_entries resb 2

vbe_current_mode:
    resb 2

; I will parse it all out in a C Struct Later
; Only .video_modes will 100% be needed in asm
vbe_info_struct:
vbe_info_signature:
    resb 4
.version:
    resb 2
.oem:
    resb 4
.capabilities:
    resb 4
video_modes_offset:   ; segment:offset to an array (little endian)
    resb 2
video_modes_segment:
    resb 2
.struct_data:
    resb 512-18

; Skipped a lot of areas,
; Not needed in ASM Code
; Will parse it out in a C Struct
vbe_mode_info_struct:
.vbe_mode_attributes:
        resb 2
.vbe_mode_misc_data:
        resb 14
.vbe_mode_pitch:
        resb 2
vbe_mode_width:
        resb 2
vbe_mode_height:
        resb 2
.vbe_mode_misc_data_2:
        resb 3
vbe_mode_bpp:
        resb 1
.vbe_mode_misc_data_3:
        resb 230

section .text

; Misc Variables stored in memory
boot_drive:
    db 0
lower_mem_size:
    dw 0

hex_conv db '0123456789ABCDEF'  ; Map for converting from binary -> ASCII Hex
KERNEL_BYTES equ 10000          ; Size of Kernel in Bytes
KERNEL_SECTORS equ 83              ; Number of Sectors the Kernel uses (ceil[number of bytes / 512])
