void *memcopy(void *dst, const void *src, unsigned long len) {
    void *ret = dst;

    __asm__ volatile (
        "cld\n"
        "rep movsb"
        : "+D"(dst), "+S"(src), "+c"(len)
        :
        : "memory"
    );

    return ret;
}

void *memmove ( void * destination, const void * source, unsigned long num );

char *strcpy ( char * destination, const char * source );

char *strcat ( char * destination, const char * source );

char *strncat ( char * destination, const char * source, unsigned long num );

int memcmp ( const void * ptr1, const void * ptr2, unsigned long num );

int strcmp ( const char * str1, const char * str2 );

void *memset ( void * ptr, int value, unsigned long num );

unsigned long strlen ( const char * str );