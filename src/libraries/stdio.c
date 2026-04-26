#include <serial.h>
#include <stdio.h>
#include <stddef.h>

/* Most of these functions are just for easy compiling for DOOM */

void printf(const char* string, ...) {
    // Temp printing to console for now
    serial_print(COM1, string);
}

void fprintf(FILE f, const char* string, ...) {
    serial_print(COM1, string);
}

// TODO:!
int snprintf(size_t size, char str[restrict size], const char *restrict format, ...) {

}