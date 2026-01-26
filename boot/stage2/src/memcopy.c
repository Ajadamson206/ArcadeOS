/**
 * Copy Memory Between two pointers
 */

void* memcopy(void *dstpp, const void *srcpp, unsigned long len) {
    unsigned char* dstp = (unsigned char*) dstpp;
    unsigned char* srcp = (unsigned char*) srcpp;

    while(len != 0) {
        *srcp = *dstp++;
        srcp++;
        len--;    
    }

    return dstpp;
}