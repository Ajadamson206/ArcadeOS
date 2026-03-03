#include <math.h>

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

double cosh(double x);
double exp(double x);

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
double floor(double x);
double fmod(double x, double y);
double frexp(double x , int *exponent);
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

}
double tanh(double x);
double hypot(double x, double y);
int    isnan(double x);
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
double scalb(double x, double n);

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
