#pragma once

#include <stdint.h>

// Suggested Inline Assembly Functions from the OSDev Wiki

/**
 * Raw Memory Access Functions
 */

/* Memory Reads */

/**
 * Read a byte from the provided segment offset
 * @param seg (u16) Segment of memory address
 * @param offset (void*) Offset of memory address
 * @return (u8) Memory at the provided value
 */
static inline u8 farpeekb(u16 seg, void* offset) {
    u8 ret;
    __asm__ volatile ( 
        "push fs\n"
        "mov fs, ax\n"
        "mov %0, byte ptr fs:[%1]\n"
        "pop fs\n"
        : "=q"(ret) 
        : "a"(seg), "r"(offset) 
    );
    
    return ret;
}

/**
 * Read a word (16 Bits) from the provided segment offset
 * @param seg (u16) Segment of memory address
 * @param offset (void*) Offset of memory address
 * @return (u16) Memory at the provided value
 */
static inline u16 farpeekw(u16 seg, void* offset) {
    u16 ret;
    __asm__ volatile ( 
        "push fs\n"
        "mov fs, ax\n"
        "mov %0, word ptr fs:[%1]\n"
        "pop fs\n"
        : "=r"(ret) 
        : "a"(seg), "r"(offset) 
    );
    
    return ret;
}

/**
 * Read a dword from the provided segment offset
 * @param seg (u16) Segment of memory address
 * @param offset (void*) Offset of memory address
 * @return (u32) Memory at the provided value
 */
static inline u32 farpeekd(u16 seg, void* offset) {
    u32 ret;
    __asm__ volatile ( 
        "push fs\n"
        "mov ax, %1\n"
        "mov fs, ax\n"
        "mov %0, dword ptr fs:[%1]\n"
        "pop fs\n"
        : "=r"(ret) 
        : "a"(seg), "r"(offset) 
    );
    
    return ret;
}

/* Memory Writes */

/**
 * Write a byte to the provided segment offset
 * @param seg (u16) Segment of memory address
 * @param offset (void*) Offset of memory address
 * @param value (u8) Value to be written
 */
static inline void farpokeb(u16 seg, const void* offset, u8 value) {
    __asm__ volatile ( 
        "push fs\n"
        "mov ax, %0\n"
        "mov fs, ax\n"
        "mov byte ptr fs:[%1], %2\n"
        "pop fs"
        : 
        : "a"(seg), "r"(offset), "q"(value) 
        : "memory"
    );
}

/**
 * Write a word to the provided segment offset
 * @param seg (u16) Segment of memory address
 * @param offset (void*) Offset of memory address
 * @param value (u16) Value to be written
 */
static inline void farpokew(u16 seg, const void* offset, u16 value) {
    __asm__ volatile ( 
        "push fs\n"
        "mov ax, %0\n"
        "mov fs, ax\n"
        "mov word ptr fs:[%1], %2\n"
        "pop fs\n"
        : 
        : "a"(seg), "r"(offset), "r"(value) 
        : "memory"
    );
}

/**
 * Write a dword to the provided segment offset
 * @param seg (u16) Segment of memory address
 * @param offset (void*) Offset of memory address
 * @param value (u32) Value to be written
 */
static inline void farpoked(u16 seg, const void* offset, u32 value) {
    __asm__ volatile ( 
        "push fs\n"
        "mov ax, %0\n"
        "mov fs, ax\n"
        "mov dword ptr fs:[%1], %2\n"
        "pop fs\n"
        : 
        : "a"(seg), "r"(offset), "r"(value) 
        : "memory"
    );
}

/**
 * I/O Access
 */

/* Input Functions */

/**
 * Read a byte from the specified port
 * @param port (u16) Specified port
 * @return (u8) Byte at the specified port
 */
static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ( 
        "in al, %1"
        : "=a"(ret)
        : "Nd"(port)
    );
    
    return ret;
}

/**
 * Read a word from the specified port
 * @param port (u16) Specified port
 * @return (u16) Word at the specified port
 */
static inline u16 inw(u16 port) {
    u16 ret;
    __asm__ volatile ( 
        "in ax, %1"
        : "=a"(ret)
        : "Nd"(port)
    );
    
    return ret;
}

/**
 * Read a dword from the specified port
 * @param port (u16) Specified port
 * @return (u32) Dword at the specified port
 */
static inline u32 ind(u16 port) {
    u32 ret;
    __asm__ volatile ( 
        "in eax, %1"
        : "=a"(ret)
        : "Nd"(port)
    );
    
    return ret;
}

/* Output Functions */

/**
 * Write a byte to a port
 * @param port (u16) Port to be written to
 * @param value (u8) Value to be written
 */
static inline void outb(u16 port, u8 value) {
    __asm__ volatile ( 
        "out %1, al" 
        : 
        : "a"(value), "Nd"(port) 
    );
}

/**
 * Write a word (16 bit value) to a port
 * @param port (u16) Port to be written to
 * @param value (u16) Value to be written
 */
static inline void outw(u16 port, u16 value) {
    __asm__ volatile ( 
        "outw %1, ax" 
        : 
        : "a"(value), "Nd"(port)
    );
}

/**
 * Write a dword (32 bit value) to a port
 * @param port (u16) Port to be written to
 * @param value (u32) Value to be written
 */
static inline void outd(u16 port, u32 value) {
    __asm__ volatile ( 
        "outd %1, eax" 
        : 
        : "a"(value), "Nd"(port)
    );
}

/* I/O Wait */

/**
 * Wait a small amount of time (Performs an IO operation on an unused port)
 */
static inline void io_wait(void) {
    outb(0x80, 0);
}

/**
 * CPU Related Functions
 */

/**
 * Read the current value stored in the CPU's time-stamp counter.
 * 
 * Contains the number of clock ticks that have occurred since the last CPU reset, and is
 * incremented after each clock cycle
 */
inline u64 rdtsc(void) {
    u64 ret;
    __asm__ volatile ( 
        "rdtsc" 
        : "=A"(ret) 
    );
    return ret;
}