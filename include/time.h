#pragma once

/**
 * Time related functions:
 * 
 * NOTE: Most function descriptions and names were copied from cppreference.com
 * for simplicity and help ensuring the functions met the spec
 */

#include <stdint.h>

#define CLOCKS_PER_SEC 1000000
#define CMOS_ADDR (u8)0x70
#define CMOS_DATA (u8)0x71
#define CURRENT_YEAR 2026

struct tm {
    int tm_sec; // seconds after the minute – [​0​, 61]
    int tm_min; // minutes after the hour – [​0​, 59]
    int tm_hour; // hours since midnight – [​0​, 23]
    int tm_mday; // day of the month – [1, 31]
    int tm_mon; // months since January – [​0​, 11]
    int tm_year; // years since 1900
    int tm_wday; // days since Sunday – [​0​, 6]
    int tm_yday; // days since January 1 – [​0​, 365]
    int tm_isdst; // Daylight Saving Time flag. The value is positive if DST is in effect, zero if not and negative if no information is available 
};

typedef u64 time_t; // Value holding the number of seconds (not counting leap seconds) since 00:00, Jan 1 1970 UTC

typedef u64 clock_t; // Value capable of representing the processor time used by a process

// Structure holding an interval broken down into seconds and nanoseconds
struct timespec {
    time_t tv_sec; // Whole Seconds
    long tv_nsec; // Nano Seconds
};

/**
 * Computes difference between two calendar times as time_t objects (time_end - time_beg) in seconds. 
 * If time_end refers to time point before time_beg then the result is negative. 
 */
extern double difftime(time_t time_end, time_t time_beg);

/**
 * Returns the current calendar time encoded as a time_t object, 
 * and also stores it in the time_t object pointed to by arg (unless arg is a null pointer) 
 */
extern time_t time(time_t* arg);

/**
 * Returns the approximate processor time used by the process since the beginning of an implementation-defined 
 * era related to the program's execution.
 */
extern clock_t clock(void);

/**
 * 1) Modifies the timespec object pointed to by ts to hold the current calendar time in the time base base. 
 * 2) Expands to a value suitable for use as the base argument of timespec_get
 * 
 * Other macro constants beginning with TIME_ may be provided by the implementation to indicate additional time bases
 * 
 * If base is TIME_UTC, then
 *  ts->tv_sec is set to the number of seconds since an implementation defined epoch, truncated to a whole value
 *  ts->tv_nsec member is set to the integral number of nanoseconds, rounded to the resolution of the system clock 
 */
extern int timespec_get( struct timespec *ts, int base );

/**
 * If ts is non-null and base is supported by timespec_get, modifies *ts to hold the 
 * resolution of time provided by timespec_get for base.
 */
extern int timespec_getres( struct timespec *ts, int base );

/**
 * Converts given calendar time tm to a textual representation of the following fixed 25-character form: Www Mmm dd hh:mm:ss yyyy
 * into the user-provided storage buffer
 */
extern int asctime_s( char* buf, u32 bufsz, const struct tm* time_ptr );

/**
 * Converts given time since epoch to a calendar local time and then to a textual representation
 */
extern int ctime_s( char *buf, u32 bufsz, const time_t* timer );

/**
 * Converts the date and time information from a given calendar time tp to a null-terminated 
 * multibyte character string str according to format string format. Up to count bytes are written. 
 */
extern u32 strftime( char* restrict str, u32 count, const char* restrict format, const struct tm* restrict tp );

/**
 * Converts given time since epoch (a time_t value pointed to by timer) into calendar time, 
 * expressed in Coordinated Universal Time (UTC) in the struct tm format and stores it
 * into a user-provided buffer
 */
extern struct tm* gmtime_r( const time_t* timer, struct tm* buf );

/**
 * Converts given time since epoch (a time_t value pointed to by timer) into calendar time, 
 * expressed in local time, in the struct tm format and store it into a user-provided buffer
 */
extern struct tm* localtime_r( const time_t* timer, struct tm* buf );

/**
 * Renormalizes local calendar time expressed as a struct tm object and also converts it to time 
 * since epoch as a time_t object. arg->tm_wday and arg->tm_yday are ignored. The values in 
 * arg are not checked for being out of range.
 * 
 * A negative value of arg->tm_isdst causes mktime to attempt to determine if Daylight Saving Time 
 * was in effect in the specified time.
 * 
 * If the conversion to time_t is successful, the arg object is modified. All fields of arg 
 * are updated to fit their proper ranges. arg->tm_wday and arg->tm_yday are recalculated using 
 * information available in other fields. 
 */
extern time_t mktime( struct tm* arg );