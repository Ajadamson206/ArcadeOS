#pragma once

#include <stdint.h>

/* Macros */
#define NULL ((void*)0)
#define RAND_MAX 0xFFFFFFFFu


/* Types */

typedef unsigned int size_t;

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

// typedef struct {
//     long long quot;
//     long long rem;
// } lldiv_t;

typedef u32 wchar_t;

static inline int isdigit(char c) {
    return (c >= '0' && c <= '9');
}

extern double atof(const char* nptr);
extern int atoi(const char* nptr);
extern long int atol(const char* nptr);
extern long long int atoll(const char* nptr);
extern int strfromd(char* restrict s, size_t n, const char* restrict format, double fp);
extern int strfromf(char* restrict s, size_t n, const char* restrict format, float fp);
extern int strfroml(char* restrict s, size_t n, const char* restrict format, long double fp);
extern double strtod(const char* restrict nptr, char** restrict endptr);
extern float strtof(const char* restrict nptr, char** restrict endptr);
extern long double strtold(const char* restrict nptr, char** restrict endptr);
extern long int strtol(const char* restrict nptr, char** restrict endptr, int base);
extern long long int strtoll(const char* restrict nptr, char** restrict endptr, int base);
extern unsigned long int strtoul(const char* restrict nptr, char** restrict endptr, int base);
extern unsigned long long int strtoull(const char* restrict nptr,
                                char** restrict endptr, int base);

/**
 * Notes for Random Number Generator:
 * Permuted Congruential Generator
 * 
 * Link: https://en.wikipedia.org/wiki/Permuted_congruential_generator
 * 
 * Two consecutive calls of rand will produce the same output
 * srand does not need to be called afterwards
 * 
 */
extern int rand(void);
extern void srand(unsigned long long seed);
extern void* aligned_alloc(size_t alignment, size_t size);
extern void* calloc(size_t nmemb, size_t size);
extern void free(void* ptr);
extern void free_sized(void* ptr, size_t size);
extern void free_aligned_sized(void* ptr, size_t alignment, size_t size);
extern void* malloc(size_t size);
extern void* realloc(void* ptr, size_t size);
[[noreturn]] extern void abort(void);
extern int atexit(void (*func)(void));
extern int at_quick_exit(void (*func)(void));
extern char* getenv(const char* name);
extern int system(const char* string);
extern void* bsearch(const void* key, void* base, size_t nmemb, size_t size,
               int (*compar)(const void* , const void* ));
extern void qsort(void* base, size_t nmemb, size_t size,
           int (*compar)(const void* , const void* ));
extern int abs(int j);
extern long int labs(long int j);
extern long long int llabs(long long int j);
extern div_t div(int numer, int denom);
extern ldiv_t ldiv(long int numer, long int denom);
// extern lldiv_t lldiv(long long int numer, long long int denom);
extern int mblen(const char* s, size_t n);
