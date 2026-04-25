#include <stdlib.h>

void *memcpy(void *dst, const void *src, unsigned long len) {
    void *ret = dst;

    unsigned long dwords = len >> 2;
    unsigned long bytes  = len & 3;

    __asm__ volatile (
        "cld\n"
        "rep movsd"
        : "+D"(dst), "+S"(src), "+c"(dwords)
        :
        : "memory"
    );

    __asm__ volatile (
        "rep movsb"
        : "+D"(dst), "+S"(src), "+c"(bytes)
        :
        : "memory"
    );

    return ret;
}

void *memmove ( void *dst, const void *src, unsigned long len ) {
    void *ret = dst;

    if (dst == src || len == 0)
        return ret;

    if ((unsigned char*)dst < (const unsigned char*)src) {
        unsigned long dwords = len >> 2;
        unsigned long bytes  = len & 3;

        __asm__ volatile (
            "cld\n"
            "rep movsd"
            : "+D"(dst), "+S"(src), "+c"(dwords)
            :
            : "memory"
        );

        __asm__ volatile (
            "rep movsb"
            : "+D"(dst), "+S"(src), "+c"(bytes)
            :
            : "memory"
        );
    } else {
        unsigned char *d = (unsigned char*)dst + len - 1;
        const unsigned char *s = (const unsigned char*)src + len - 1;

        __asm__ volatile (
            "std\n"
            "rep movsb\n"
            "cld"
            : "+D"(d), "+S"(s), "+c"(len)
            :
            : "memory"
        );
    }

    return ret;
}

char *strcpy ( char * destination, const char * source ) {
    char *ret = destination;

    while(*source) {
        *destination++ = *source++;
    }
    *destination = 0;
    
    return ret;
}

char *strcat ( char * destination, const char * source ) {
    char *ret = destination;

    // Look for null byte
    while(*destination++);

    // Found Null Byte
    while(*source) {
        *destination++ = *source++;
    }
    *destination = 0;


    return ret;
}

char *strncat ( char * destination, const char * source, unsigned long num );

int memcmp ( const void * ptr1, const void * ptr2, unsigned long num );

int strcmp ( const char * str1, const char * str2 ) {
    while(*str1 && *str2) {
        if(*str1 - *str2)
            return *str1 - *str2;
        str1++;
        str2++;
    }

    return (int)(*str1 - *str2);
}

void *memset( void *ptr, int value, unsigned long num ) {
    void *ret = ptr;

    unsigned char byte = (unsigned char)value;
    unsigned long pattern = (unsigned long)byte;
    pattern |= pattern << 8;
    pattern |= pattern << 16;

    unsigned long dwords = num >> 2;
    unsigned long bytes = num & 3;

    __asm__ volatile (
        "cld\n"
        "rep stosd"
        : "+D"(ptr), "+c"(dwords)
        : "a"(pattern)
        : "memory"
    );

    __asm__ volatile (
        "rep stosb"
        : "+D"(ptr), "+c"(bytes)
        : "a"(byte)
        : "memory"
    );

    return ret;
}

void *memset16( void *ptr, int value, unsigned long num_bytes ) {
    void *ret = ptr;

    unsigned short word = (unsigned short)value;
    unsigned char byte = (unsigned char)word;
    unsigned long pattern = (unsigned long)word;
    pattern |= pattern << 16;

    unsigned long dwords = num_bytes >> 2;
    unsigned long words = (num_bytes & 2) >> 1;
    unsigned long bytes = num_bytes & 1;

    // Copy 32 bits
    __asm__ volatile (
        "cld\n"
        "rep stosd"
        : "+D"(ptr), "+c"(dwords)
        : "a"(pattern)
        : "memory"
    );

    // Copy 16 bits
    __asm__ volatile (
        "rep stosw"
        : "+D"(ptr), "+c"(words)
        : "a"(word)
        : "memory"
    );

    // Copy 8 bits
    __asm__ volatile (
        "rep stosb"
        : "+D"(ptr), "+c"(bytes)
        : "a"(byte)
        : "memory"
    );

    return ret;
}

void *memset32( void *ptr, int value, unsigned long num_bytes ) {
    void *ret = ptr;

    unsigned long dwords = num_bytes >> 2;
    unsigned long words = (num_bytes & 2) >> 1;
    unsigned long bytes = num_bytes & 1;
    
    unsigned long pattern = (unsigned long)value;
    unsigned short word = (unsigned short)(pattern);
    unsigned char byte = (unsigned char)((words)? (pattern >> 16) : pattern);


    // Copy 32 bits
    __asm__ volatile (
        "cld\n"
        "rep stosd"
        : "+D"(ptr), "+c"(dwords)
        : "a"(pattern)
        : "memory"
    );

    // Copy 16 bits
    __asm__ volatile (
        "rep stosw"
        : "+D"(ptr), "+c"(words)
        : "a"(word)
        : "memory"
    );

    // Copy 8 bits
    __asm__ volatile (
        "rep stosb"
        : "+D"(ptr), "+c"(bytes)
        : "a"(byte)
        : "memory"
    );

    return ret;
}

unsigned long strlen ( const char * str ) {
    unsigned long len = 0;
    while(*str++) {
        len++;
    }

    return len;
}

char * strrchr(const char * string, int character) {
    char *ret = (void *)(0);

    while(*string) {
        if(*string == (char)character) {
            ret = (char *)string;
        }
        string++;
    }

    // Check if character is 0
    if(*string == character)
        ret = (char *)string;

    return ret;
}

char *strdup(const char *str1) {
    u32 len = (u32)strlen(str1);

    char *new_str = malloc(len * sizeof(*new_str));    
    char *ret = new_str;

    if(new_str == NULL)
        return NULL;

    while(*str1) {
        *new_str++ = *str1++;
    }
    *new_str = 0;

    return ret;
}