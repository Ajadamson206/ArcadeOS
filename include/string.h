#pragma once

/**
 * Copy len bytes from src to dst
 * NOTE: If the pointers overlap it can lead to undefined behavior
 * @param dst (void *) Pointer to where the memory should be copied to
 * @param src (const void *) Pointer to where the input is
 * @param len (unsigned long) Length in bytes of the src memory 
 */
extern void *memcopy(void *dst, const void *src, unsigned long len);

/**
 * Copies the values of num bytes from the location pointed by source to the 
 * memory block pointed by destination. Copying takes place as if an intermediate 
 * buffer were used, allowing the destination and source to overlap.
 */
extern void *memmove(void * destination, const void * source, unsigned long num);

/**
 * Copies the C string pointed by source into the array pointed by destination, 
 * including the terminating null character (and stopping at that point).
 * 
 * To avoid overflows, the size of the array pointed by destination shall be 
 * long enough to contain the same C string as source (including the terminating 
 * null character), and should not overlap in memory with source.
 */
extern char *strcpy(char * destination, const char * source);

/**
 * Appends a copy of the source string to the destination string. The terminating 
 * null character in destination is overwritten by the first character of source, 
 * and a null-character is included at the end of the new string formed by the 
 * concatenation of both in destination.
 * 
 * destination and source shall not overlap.
 */
extern char *strcat(char * destination, const char * source);

/**
 * Appends the first num characters of source to destination, plus a terminating 
 * null-character.If the length of the C string in source is less than num, only 
 * the content up to the terminating null-character is copied.
 */
extern char *strncat(char * destination, const char * source, unsigned long num);

/**
 * Compares the first num bytes of the block of memory pointed by ptr1 to the first 
 * num bytes pointed by ptr2, returning zero if they all match or a value different 
 * from zero representing which is greater if they do not.
 */
extern int memcmp(const void * ptr1, const void * ptr2, unsigned long num);

/**
 * Compares the C string str1 to the C string str2.
 */
extern int strcmp(const char * str1, const char * str2);

/**
 * Sets the first num bytes of the block of memory pointed by ptr to the specified 
 * value (interpreted as an unsigned char).
 */
extern void *memset(void * ptr, int value, unsigned long num);

/**
 * Returns the length of the C string str.
 */
extern unsigned long strlen(const char * str);