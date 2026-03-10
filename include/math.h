#pragma once

#define M_E		    2.718281828459045235360287471352662498 /* e */
#define M_LOG2E	    1.442695040888963407359924681001892137 /* log_2 e */
#define M_LOG10E	0.434294481903251827651128918916605082 /* log_10 e */
#define M_LN2		0.693147180559945309417232121458176568 /* log_e 2 */
#define M_LN10	    2.302585092994045684017991454684364208 /* log_e 10 */
#define M_PI		3.141592653589793238462643383279502884 /* pi */
#define M_PI_2	    1.570796326794896619231321691639751442 /* pi/2 */
#define M_PI_4	    0.785398163397448309615660845819875721 /* pi/4 */
#define M_1_PI	    0.318309886183790671537767526745028724 /* 1/pi */
#define M_2_PI	    0.636619772367581343075535053490057448 /* 2/pi */
#define M_2_SQRTPI	1.128379167095512573896158903121545172 /* 2/sqrt(pi) */
#define M_SQRT2	    1.414213562373095048801688724209698079 /* sqrt(2) */
#define M_SQRT1_2	0.707106781186547524400844362104849039 /* 1/sqrt(2) */

#define M_Ef		2.7182818284590452354f	/* e */
#define M_LOG2Ef	1.4426950408889634074f	/* log_2 e */
#define M_LOG10Ef	0.43429448190325182765f	/* log_10 e */
#define M_LN2f		0.69314718055994530942f	/* log_e 2 */
#define M_LN10f	    2.30258509299404568402f	/* log_e 10 */
#define M_PIf		3.14159265358979323846f	/* pi */
#define M_PI_2f	    1.57079632679489661923f	/* pi/2 */
#define M_PI_4f	    0.78539816339744830962f	/* pi/4 */
#define M_1_PIf	    0.31830988618379067154f	/* 1/pi */
#define M_2_PIf	    0.63661977236758134308f	/* 2/pi */
#define M_2_SQRTPIf	1.12837916709551257390f	/* 2/sqrt(pi) */
#define M_SQRT2f	1.41421356237309504880f	/* sqrt(2) */
#define M_SQRT1_2f	0.70710678118654752440f	/* 1/sqrt(2) */

/**
 * Returns the arc cosine of x in radians
 */
extern double acos(double x);
extern double asin(double x);
extern double atan(double x);
extern double ceil(double x);
extern double cos(double x);
extern double cosh(double x);
extern double exp(double x);
extern double fabs(double x);
extern double floor(double x);

/**
 * Returns the remainder of x divided by y.
 */
extern double fmod(double x, double y);

/**
 * The returned value is the mantissa and the integer pointed to by exponent is the exponent. The resultant value is x = mantissa * 2 ^ exponent.
 */
extern double frexp(double x , int *exponent);

/**
 * Returns x multiplied by 2 raised to the power of exponent.
 */
extern double ldexp(double x, int exponent);

/**
 * Compute ln(x)
 */
extern double log(double x);
extern double log10(double x);

/**
 * The returned value is the fraction component (part after the decimal), and sets integer to the integer component.
 */
extern double modf(double x, double *integer);

/**
 * Returns x raised to the y
 */
extern double pow(double x, double y);
extern double sin(double x);
extern double sinh(double x);
extern double sqrt(double x);
extern double tan(double x);
extern double tanh(double x);

/**
 * Computes the hypotenuse of a right-angled triangle
 */
extern double hypot(double x, double y);
extern int    isnan(double x);

/**
 * Computes cube root of x
 */
extern double cbrt(double x);

/**
 * The remainder() function returns the floating point remainder r = x - ny when y is non-zero. 
 * The value n is the integral value nearest the exact value x/y. When | n - x/y |  = ½ , 
 * the value n is chosen to be even.The behaviour of remainder() is independent of the rounding mode. 
 */
extern double remainder(double x, double y);

/**
 * The rint() function returns the integral value (represented as a double) 
 * nearest x in the direction of the current rounding mode. The current rounding 
 * mode is implementation-dependent. 
 */
extern double rint(double x);

/**
 * The scalb() function computes x * (r^n), where r is the radix of the machine's floating 
 * point arithmetic. When r is 2, scalb() is equivalent to ldexp(). 
 */
extern double scalb(double x, double n);


/* Float Functions */


/**
 * Returns the arc cosine of x in radians
 */
extern float acosf(float x);
extern float asinf(float x);
extern float atanf(float x);

/**
 * The atan2() function computes the principal value of the arc tangent of y/x, using the signs of both arguments to determine the quadrant of the return value. 
 */
extern float atan2f(float y, float x);
extern float ceilf(float x);
extern float cosf(float x);
extern float coshf(float x);
extern float expf(float x);
extern float fabsf(float x);
extern float floorf(float x);

/**
 * Returns the remainder of x divided by y.
 */
extern float fmodf(float x, float y);

/**
 * The returned value is the mantissa and the integer pointed to by exponent is the exponent. The resultant value is x = mantissa * 2 ^ exponent.
 */
extern float frexpf(float x , int *exponent);

/**
 * Returns x multiplied by 2 raised to the power of exponent.
 */
extern float ldexpf(float x, int exponent);
extern float logf(float x);
extern float log10f(float x);

/**
 * The returned value is the fraction component (part after the decimal), and sets integer to the integer component.
 */
extern float modff(float x, float *integer);

/**
 * Returns x raised to the y
 */
extern float powf(float x, float y);
extern float sinf(float x);
extern float sinhf(float x);
extern float sqrtf(float x);
extern float tanf(float x);
extern float tanhf(float x);

/**
 * Computes the hypotenuse of a right-angled triangle
 */
extern float hypotf(float x, float y);
extern int    isnanf(float x);
extern float acoshf(float x);
extern float asinhf(float x);
extern float atanhf(float x);

/**
 * Computes cube root of x
 */
extern float cbrtf(float x);

/**
 * The nextafter() function computes the next representable double-precision floating-point 
 * value following x in the direction of y. Thus, if y is less than x, nextafter() 
 * returns the largest representable floating-point number less than x. 
 */
extern float nextafterf(float x, float y);

/**
 * The remainder() function returns the floating point remainder r = x - ny when y is non-zero. 
 * The value n is the integral value nearest the exact value x/y. When | n - x/y |  = ½ , 
 * the value n is chosen to be even.The behaviour of remainder() is independent of the rounding mode. 
 */
extern float remainderf(float x, float y);

/**
 * The rint() function returns the integral value (represented as a double) 
 * nearest x in the direction of the current rounding mode. The current rounding 
 * mode is implementation-dependent. 
 */
extern float rintf(float x);

/**
 * The scalb() function computes x * (r^n), where r is the radix of the machine's floating 
 * point arithmetic. When r is 2, scalb() is equivalent to ldexp(). 
 */
extern float scalbf(float x, float n);