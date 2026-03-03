#include <stdint.h>
#include <time.h>
#include <rawio.h>
#include <math.h>

static int century_register = 0x00;

static u8 second;
static u8 minute;
static u8 hour;
static u8 day;
static u8 month;
static u32 year;
static u8 century;

// Check if the BIOS is updating the clock
static u8 get_update_in_progress_flag() {
    outb(CMOS_ADDR, 0x0A);
    return (inb(CMOS_DATA) & 0x80);
}

// Read a RTC Register
static u8 get_RTC_register(u8 reg) {
    outb(CMOS_ADDR, reg);
    return inb(CMOS_DATA);
}

// Read + Store the values in the RTC to the static variables
static void read_rtc() {
    // Don't Read During an Update
    while (get_update_in_progress_flag());
    
    second = get_RTC_register(0x00);
    minute = get_RTC_register(0x02);
    hour = get_RTC_register(0x04);
    day = get_RTC_register(0x07);
    month = get_RTC_register(0x08);
    year = get_RTC_register(0x09);
    
    if(century_register != 0) {
        century = get_RTC_register(century_register);
    }

    u8 registerB = get_RTC_register(0x0B);

    // Convert BCD to binary values if necessary
    if(!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
        if(century_register != 0) {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }

    // Convert 12 hour clock to 24 hour clock if necessary
    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }

    // Calculate the full (4-digit) year
    if(century_register != 0) {
        year += century * 100;
    } else {
        year += (CURRENT_YEAR / 100) * 100;
        if(year < CURRENT_YEAR) year += 100;
    }

}

/* Time Manipulation Functions */

double difftime(time_t time_end, time_t time_beg) {
    return (double)(((u64)time_end) - ((u64)time_beg));
}

static inline int is_leapyear(u32 year) {
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

static u32 num_days(void) {
    u32 num = 0;
    
    // Do not include the current month
    switch (month)
    {
        case 12: // Calc November
            num += 30;
        case 11:
            num += 31;
        case 10:
            num += 30;
        case 9:
            num += 31;
        case 8:
            num += 31;
        case 7:
            num += 30;
        case 6:
            num += 31;
        case 5:
            num += 30;
        case 4:
            num += 31;
        case 3: // Calc Feb
            if(is_leapyear(CURRENT_YEAR)) {
                num += 1;
            }
            num += 28;
        case 2: // 31 Days in Jan
            num += 31; 
            break;
    }
    
    return num;
}

time_t time(time_t* arg) {
    // Store the current time
    read_rtc();

    time_t output = 0;

    // Years -> Seconds
    u32 ys = (u32)floor((double)(year - 1970) * 365.2422); // Include Leap Years
    ys *= 24 * 60 * 60;

    // Month -> Days
    u32 days = num_days();
    days += day - 1; // Do not include current Day

    // Days -> Hours
    u32 hours = days * 24;
    hours += hour;

    // Hours -> Minutes
    u64 minutes = hours * 60;
    minutes += minute;

    // Minutes -> Seconds
    output += minutes * 60;
    output += second;

    // Output
    if(arg) {
        *arg = output;
    }

    return output;
}

clock_t clock(void) {
    return (clock_t)rdtsc();
}

int timespec_get( struct timespec *ts, int base );
int timespec_getres( struct timespec *ts, int base );

/* Time Format Conversions */

int asctime_s( char* buf, u32 bufsz, const struct tm* time_ptr );
int ctime_s( char *buf, u32 bufsz, const time_t* timer );
u32 strftime( char* restrict str, u32 count, const char* restrict format, const struct tm* restrict tp );
struct tm* gmtime_r( const time_t* timer, struct tm* buf );
struct tm* localtime_r( const time_t* timer, struct tm* buf );
time_t mktime( struct tm* arg );