/**
 * Copy Memory Between two pointers
 */
#include <stdint.h>

void* memcopy(void *dstpp, const void *srcpp, unsigned long len) {
    u32* dstp = (u32*) dstpp;
    const u32* srcp = (const u32*) srcpp;    

    // Start with 32 bit (4 Byte) chunks
    unsigned long chunks = len / 4;
    unsigned long rem = len % 4;
    
    while(chunks--) {
        *dstp++ = *srcp++;
    }

    // Finish the rest with 8 Bit (1 Byte) Chunks
    unsigned char* dste = (unsigned char*) dstp;
    const unsigned char* srce = (const unsigned char*) srcp;

    while(rem--) {
        *dste++ = *srce++;
    }

    return dstpp;
}