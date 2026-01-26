__attribute__((noreturn))
void stage2_c_main(void) {
    volatile unsigned short *vga = (unsigned short*)0xB8000;
    vga[2] = 0x2F43; // 'C'
    for(;;) { __asm__ volatile ("hlt"); }
}