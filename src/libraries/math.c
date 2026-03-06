#include <math.h>

static union {
    unsigned long long x;
    double y;
} conv;

_Static_assert(sizeof(unsigned long long) == sizeof(double), "Math.c union conv: x != y");

double acos(double x) {

}
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double ceil(double x);

double cos(double x) {
    __asm__ volatile (
        "fld %1\n"
        "fcos\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x)
        : "st"
    );

    return x;
}

double cosh(double x) {
    double e_x = exp(x);
    return (e_x + (1.0 / e_x)) / 2.0;
}

//e^x = e^(k*ln(2) + r) = 2^k * e^r
// Modified from: 
// https://justinwillmert.com/articles/2020/numerically-computing-the-exponential-function-with-polynomial-approximations/
double exp(double x) {
    unsigned long long k = (unsigned long long)((x / M_LN2) + (0.5));
    double r = x - ((double)k * M_LN2);

    // Use 4th degree Polynomial Approximation
    double ret = (1 + r * (1 + (r / 2.0) * (1 + (r / 3.0) * (1 + (r / 4.0)))));

    // Prevent ULL Overflows
    while(k >= 64) {
        k -= 63;
        ret *= (double)(1ULL<<63);
    }
        
    return (double)(1ULL<<k) * ret;
}

double fabs(double x) {
    __asm__ volatile (
        "fld %1\n"
        "fabs\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x)
        : "st"
    );

    return x;
}
double floor(double x) {
    return x;
}
double fmod(double x, double y);

// NEEDS TO BE REVISITED
double frexp(double x , int *exponent) {
    // Remove sign bit and Mantisa
    conv.y = x;
    conv.x &= 0x7FF0000000000000ULL;

    // Extract Exponent
    *exponent = conv.x >> 52;

    // Return Mantisa + Sign Bit
    conv.y = x;
    return conv.x & 0x800FFFFFFFFFFFFFULL;
}

double ldexp(double x, int exponent);
double log(double x);
double log10(double x);
double modf(double x, double *integer);
double pow(double x, double y);

double sin(double x) {
    __asm__ volatile (
        "fld %1\n"
        "fsin\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x)
        : "st"
    );

    return x;
}

double sinh(double x) {
    double e_x = exp(x);
    return (e_x - (1.0 / e_x)) / 2.0;
}

double sqrt(double x) {
    __asm__ volatile (
        "fld %1\n"
        "fsqrt\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x)
        : "st"
    );

    return x;
}

double tan(double x) {
    __asm__ volatile (
        "fld %1\n"
        "fptan\n"
        "fcomp\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x)
        : "st"
    );

    return x;
}

double tanh(double x) {
    double e_x = exp(2.0 * x);
    return (e_x - 1) / (e_x + 1); 
}

double hypot(double x, double y) {
    __asm__ volatile (
        "fld %1\n"
        "fld %1\n"
        "fmulp\n"
        "fld %2\n"
        "fld %2\n"
        "fmulp\n"
        "faddp\n"
        "fsqrt\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x), "m"(y)
        : "st"
    );

    return x;
}

int isnan(double x) {
    // All exponents are 1 / Fraction is anything but 0
    conv.y = x;

    conv.x &= 0x7FFFFFFFFFFFFFFFULL;
    
    // Set the exponents to 1 and the fraction to 0 (Ignore sign)
    // 0b0111111111110000000000000000000000000000000000000000000000000000
    return conv.x > 0x7FF0000000000000ULL;
}

double acosh(double x);
double asinh(double x);
double atanh(double x);
double cbrt(double x);
double nextafter(double x, double y);

double remainder(double x, double y) {
    __asm__ volatile (
        "fld %2\n"
        "fld %1\n"
        "fprem\n"
        "fstp %0\n"
        : "=m"(x)
        : "m"(x), "m"(y)
        : "st"
    );

    return x;
}

double rint(double x);
double scalb(double x, double n) {
    __asm__ volatile (
        "fld %2\n"
        "fld %1\n"
        "fscale\n"
        "fstp %0\n"
        : "=m"(x)
        : "m"(x), "m"(n)
        : "st"
    );

    return x;
}

/* Float Functions */

float acosf(float x);
float asinf(float x);
float atanf(float x);
float atan2f(float y, float x);
float ceilf(float x);
float cosf(float x);
float coshf(float x);
float expf(float x);
float fabsf(float x);
float floorf(float x);
float fmodf(float x, float y);
float frexpf(float x , int *exponent);
float ldexpf(float x, int exponent);
float logf(float x);
float log10f(float x);
float modff(float x, float *integer);
float powf(float x, float y);
float sinf(float x);
float sinhf(float x);
float sqrtf(float x);
float tanf(float x);
float tanhf(float x);
float hypotf(float x, float y);
int    isnanf(float x);
float acoshf(float x);
float asinhf(float x);
float atanhf(float x);
float cbrtf(float x);
float nextafterf(float x, float y);
float remainderf(float x, float y);
float rintf(float x);
float scalbf(float x, float n);
