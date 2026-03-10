#include <math.h>

static union {
    unsigned long long x;
    double y;
} conv;

_Static_assert(sizeof(unsigned long long) == sizeof(double), "Math.c union conv: x != y");

double acos(double x) {
    return M_PI_2 - asin(x);
}

double asin(double x) {
    double y = x;

    for(int i = 0; i < 5; i++)
        y = y - (sin(y) - x) / cos(y);

    return y;
}

double atan(double x) {
    return asin(x / sqrt(1 + (x * x)));
}


double ceil(double x) {
    long long i = (long long)x;

    if (x > 0 && x != i)
        i++;

    return (double)i;
}

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
    int neg = 0;
    if(x < 0) {
        neg = 1;
        x *= -1.0;
    }

    unsigned long long k = (unsigned long long)((x / M_LN2) + (0.5));
    double r = x - ((double)k * M_LN2);

    // Use 4th degree Polynomial Approximation
    double ret = (1 + r * (1 + (r / 2.0) * (1 + (r / 3.0) * (1 + (r / 4.0)))));

    // Prevent ULL Overflows
    while(k >= 64) {
        k -= 63;
        ret *= (double)(1ULL<<63);
    }

    if(neg)
        return 1.0 / ((double)(1ULL<<k) * ret);
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
    long long i = (long long)x;

    if (x < 0 && x != i)
        i--;

    return (double)i;
}

double fmod(double x, double y) {
    __asm__ volatile (
        "fld %2\n"
        "fld %1\n"
        "fprem\n"
        "fstp st(1)\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x), "m"(y)
        : "st"
    );

    return x;
}

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

// Approximate Arithmatic Geometric Mean
static double agm(double x, double y) {
    double a = 0.5 * (x + y);
    double b = sqrt(x * y);
    x = 0.5 * (a + b);
    y = sqrt(a * b);
    a = 0.5 * (x + y);
    b = sqrt(x * y);
    x = 0.5 * (a + b);
    y = sqrt(a * b);

    return 0.5 * (x + y);
}


double ldexp(double x, int exponent);

double log(double x) {
    return (M_PI / (2.0 * agm(1.0, (4.0 / (x * 256.0))))) - (8.0 * M_LN2);
}

double log10(double x);
double modf(double x, double *integer);

// x^y = e^(y * ln(x))
double pow(double x, double y) {
    if(x > 1.0) {
        return exp(y * log(x));
    }
}

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
        "fld st(0)\n"
        "fmulp st(1), st(0)\n"
        "fld %2\n"
        "fld st(0)\n"
        "fmulp st(1), st(0)\n"
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

double cbrt(double x) {
    // Use Halley's Method 4 Times a_n+1 = a_n ( (a_n^3 + 2x) / (2a_n^3 + x))
    double a = 1.0;
    a = a * ( ((a * a * a )+ (2.0 * x)) / ((2.0 * a * a * a) + x));
    a = a * ( ((a * a * a )+ (2.0 * x)) / ((2.0 * a * a * a) + x));
    a = a * ( ((a * a * a )+ (2.0 * x)) / ((2.0 * a * a * a) + x));
    a = a * ( ((a * a * a )+ (2.0 * x)) / ((2.0 * a * a * a) + x));

    return a;
}


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

double rint(double x) {
    __asm__ volatile (
        "fld %1\n"
        "frndint\n"
        "fstp %0\n"
        : "=m"(x)
        : "m"(x)
        : "st"
    );

    return x;
}

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

float acosf(float x) {
    return M_PI_2f - asinf(x);
}

float asinf(float x) {
    float y = x;

    for(int i = 0; i < 5; i++)
        y = y - (sinf(y) - x) / cosf(y);

    return y;
}

float atanf(float x) {
    return asinf(x / sqrtf(1.0f + (x * x)));
}

float ceilf(float x) {
    long long i = (long long)x;

    if (x > 0 && x != i)
        i++;

    return (float)i;
}

float cosf(float x) {
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

float coshf(float x) {
    float e_x = expf(x);
    return (e_x + (1.0f / e_x)) / 2.0f;
}

float expf(float x) {
    int neg = 0;
    if(x < 0.0f) {
        neg = 1;
        x *= -1.0f;
    }

    unsigned long k = (unsigned long)((x / M_LN2f) + (0.5f));
    float r = x - ((float)k * M_LN2f);

    // Use 4th degree Polynomial Approximation
    float ret = (1.0f + r * (1.0f + (r / 2.0f) * (1.0f + (r / 3.0f) * (1.0f + (r / 4.0f)))));

    // Prevent ULL Overflows
    while(k >= 32) {
        k -= 31;
        ret *= (float)(1UL<<31);
    }

    if(neg)
        return 1.0f / ((float)(1UL<<k) * ret);
    return (float)(1UL<<k) * ret;
}

float fabsf(float x) {
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

float floorf(float x) {
    long long i = (long long)x;

    if (x < 0 && x != i)
        i--;

    return (float)i;
}

float fmodf(float x, float y) {
    __asm__ volatile (
        "fld %2\n"
        "fld %1\n"
        "fprem\n"
        "fstp st(1)\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x), "m"(y)
        : "st"
    );

    return x;
}

float frexpf(float x , int *exponent);
float ldexpf(float x, int exponent);

// Approximate Arithmatic Geometric Mean
static float agmf(float x, float y) {
    float a = 0.5f * (x + y);
    float b = sqrtf(x * y);
    x = 0.5f * (a + b);
    y = sqrtf(a * b);
    a = 0.5f * (x + y);
    b = sqrtf(x * y);
    x = 0.5f * (a + b);
    y = sqrtf(a * b);

    return 0.5 * (x + y);
}

float logf(float x) {
    return (M_PIf / (2.0f * agmf(1.0f, (4.0f / (x * 256.0f))))) - (8.0f * M_LN2f);
}
float log10f(float x);
float modff(float x, float *integer);

float powf(float x, float y);

float sinf(float x) {
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

float sinhf(float x) {
    float e_x = expf(x);
    return (e_x - (1.0f / e_x)) / 2.0f;
}

float sqrtf(float x) {
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

float tanf(float x) {
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

float tanhf(float x) {
    float e_x = expf(2.0f * x);
    return (e_x - 1.0f) / (e_x + 1.0f); 
}

float hypotf(float x, float y) {
        __asm__ volatile (
        "fld %1\n"
        "fld st(0)\n"
        "fmulp st(1), st(0)\n"
        "fld %2\n"
        "fld st(0)\n"
        "fmulp st(1), st(0)\n"
        "faddp\n"
        "fsqrt\n"
        "fstp %0\n"
        : "=m" (x)
        : "m" (x), "m"(y)
        : "st"
    );

    return x;
}

int    isnanf(float x);


float cbrtf(float x) {
    // Use Halley's Method 4 Times a_n+1 = a_n ( (a_n^3 + 2x) / (2a_n^3 + x))
    float a = 1.0f;
    a = a * ( ((a * a * a )+ (2.0f * x)) / ((2.0f * a * a * a) + x));
    a = a * ( ((a * a * a )+ (2.0f * x)) / ((2.0f * a * a * a) + x));
    a = a * ( ((a * a * a )+ (2.0f * x)) / ((2.0f * a * a * a) + x));
    a = a * ( ((a * a * a )+ (2.0f * x)) / ((2.0f * a * a * a) + x));

    return a;
}

float remainderf(float x, float y) {
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

float rintf(float x) {
    __asm__ volatile (
        "fld %1\n"
        "frndint\n"
        "fstp %0\n"
        : "=m"(x)
        : "m"(x)
        : "st"
    );

    return x;
}

float scalbf(float x, float n) {
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
