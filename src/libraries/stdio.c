#include <serial.h>

void printf(const char* string, ...) {
    // Temp printing to console for now
    serial_print(COM1, string);
}