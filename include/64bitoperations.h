#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * All of the names come from gcc
 */


// Cause Infinite Loop if divide by 0
static void __divide_zero_trap(void) {
    for (;;)
        __asm__ volatile("hlt");
}

/**
 * Unsigned 64-Bit division with optional remainder
 * @param numerator (u64) Numerator in the division
 * @param denominator (u64) Denominator in the division
 * @param rem (u64 *) Pointer to where the remainder should be stored
 * @returns numerator / denominator
 * @returns if rem != NULL, *rem = numerator % denominator
 */
u64 __udivmoddi4(u64 numerator, u64 denominator, u64 *rem) {
    u64 quotient = 0;
    u64 qbit = 1;

    if (denominator == 0) {
        __divide_zero_trap();
    }

    
    // Align denominator to the highest useful bit.
    // Stop shifting if the top bit would be lost.
    while ((denominator < numerator) && ((denominator & (1ULL << 63)) == 0)) {
        denominator <<= 1;
        qbit <<= 1;
    }

    // Long Division
    while (qbit != 0) {
        if (numerator >= denominator) {
            numerator -= denominator;
            quotient |= qbit;
        }
        denominator >>= 1;
        qbit >>= 1;
    }

    if (rem != NULL) {
        *rem = numerator;
    }

    return quotient;
}

/**
 * Regular Unsigned Division without remainder
 */
u64 __udivdi3(u64 a, u64 b) {
    return __udivmoddi4(a, b, (u64 *)0);
}