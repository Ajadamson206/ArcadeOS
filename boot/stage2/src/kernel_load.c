// Need to load the kernel into address 0x00200000

/**
 * Step 1: Enter Real Mode again
 * 
 * Step 2: Load Kernel into 0xA000
 * 
 * Step 3: Subtract the number of sectors loaded
 * 
 * Step 4: Go back to 32-bit mode (Protected Mode)
 * 
 * Step 5: Move it to 0x00200000 + (Number of sectors already loaded * 512)
 * 
 * Step 6: Repeat until everything is loaded 
 * 
 * Step 7: Verify it is loaded correctly
 */

void load_kernel() {
    __asm__ volatile(
        "call c_code_exit"
        :
        :
        : "memory"
    );
}