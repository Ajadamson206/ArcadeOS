#pragma once

// Use a char* so we can increment one byte at a time
typedef char *va_list;

// Ensures that we advance across the stack correctly
#define __va_size(type) \
    (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

// Sets args to the address just after that last fixed argument, last_fixed_arg
#define va_start(ap, last_fixed_arg) \
    ((ap) = (va_list)&(last_fixed_arg) + __va_size(last_fixed_arg))

// Returns the current argument and advances the pointer to the next one
#define va_arg(ap, type) \
    (*(type *)(((ap) += __va_size(type)) - __va_size(type)))

// Just set the pointer to zero
#define va_end(ap) \
    ((ap) = (va_list)0)

// Basic Copy
#define va_copy(dest, src) \
    ((dest) = (src))