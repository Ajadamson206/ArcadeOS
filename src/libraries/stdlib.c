#include <stdlib.h>
#include <stdint.h>
#include <rawio.h>

// Convert string -> float
double atof(const char* nptr) {
    // Get Left side of decimal first
    double output = 0.0;


    return output;
}

// Convert string -> int
int atoi(const char* nptr) {
    int output = 0;
    int sign = 1;

    while(!isdigit(*nptr) && (*nptr != '-')) {
        nptr++;
    }

    if(*nptr == '-') {
        nptr++;
        sign = -1;
    }

    while(*nptr) {
        if(isdigit(*nptr++)) {
            output += *nptr - '0';
            output *= 10;
        }
    }

    return output * sign;
}

long int atol(const char* nptr) {
    long int output = 0;
    long int sign = 1;

    while(!isdigit(*nptr) && (*nptr != '-')) {
        nptr++;
    }

    if(*nptr == '-') {
        nptr++;
        sign = -1;
    }

    while(*nptr) {
        if(isdigit(*nptr++)) {
            output += *nptr - '0';
            output *= 10;
        }
    }

    return output * sign;
}

long long int atoll(const char* nptr) {
    long long int output = 0;
    long long int sign = 1;

    while(!isdigit(*nptr) && (*nptr != '-')) {
        nptr++;
    }

    if(*nptr == '-') {
        nptr++;
        sign = -1;
    }

    while(*nptr) {
        if(isdigit(*nptr++)) {
            output += *nptr - '0';
            output *= 10;
        }
    }

    return output * sign;
}
int strfromd(char* restrict s, size_t n, const char* restrict format, double fp);
int strfromf(char* restrict s, size_t n, const char* restrict format, float fp);
int strfroml(char* restrict s, size_t n, const char* restrict format, long double fp);
double strtod(const char* restrict nptr, char** restrict endptr);
float strtof(const char* restrict nptr, char** restrict endptr);
long double strtold(const char* restrict nptr, char** restrict endptr);
long int strtol(const char* restrict nptr, char** restrict endptr, int base);
long long int strtoll(const char* restrict nptr, char** restrict endptr, int base);
unsigned long int strtoul(const char* restrict nptr, char** restrict endptr, int base);
unsigned long long int strtoull(const char* restrict nptr,
                                char** restrict endptr, int base);


/* Randomly Generated Values */

static u64 state = 0x5ECF69EFCA182FE3ULL;
static u64 const multiplier = 0x7443BF8C20D3B2FEULL;
static u64 const increment = 0xB4A84D7E859B2FD5ULL;

int rand(void) {
    u64 x = state;
	u32 count = (u32)(x >> 59);

	state = x * multiplier + increment;
	x ^= x >> 18;

	return x >> count | x << (-count & 31);
}

void srand(unsigned long long seed) {
    state = seed + increment;
}

void* aligned_alloc(size_t alignment, size_t size);
void* calloc(size_t nmemb, size_t size);
void free(void* ptr);
void free_sized(void* ptr, size_t size);
void free_aligned_sized(void* ptr, size_t alignment, size_t size);
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);

static void (*exit_func)(void) = NULL;

__attribute__((noreturn))
void abort(void) {
    if(exit_func) {
        exit_func();
    }

    // Power off QEMU Virtual Machine
    outw(0x604, 0x2000);

    __builtin_unreachable();
}

int atexit(void (*func)(void)) {
    exit_func = func;

    return 0;
}

char* getenv(const char* name) {
    return NULL;
}

void* bsearch(const void* key, void* base, size_t nmemb, size_t size,
               int (*compar)(const void* , const void* ));

void qsort(void* base, size_t nmemb, size_t size,
           int (*compar)(const void* , const void* ));

int abs(int j);
long int labs(long int j);
long long int llabs(long long int j);
div_t div(int numer, int denom);
ldiv_t ldiv(long int numer, long int denom);
lldiv_t lldiv(long long int numer, long long int denom);