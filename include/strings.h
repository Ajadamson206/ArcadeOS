#pragma once

#include <stddef.h>
#include <ctype.h>

static char __to_lower(char c) {
    if(isalpha(c))
        return (c % 32) + '`';
    
    return c;
}

int strcasecmp(const char *s1, const char *s2) {
    while(*s1 && *s2) {
        char l = __to_lower(*s1++);
        char r = __to_lower(*s2++);

        // Check for the end of the string
        if(!l || !r)
            return (int)(l - r);

        // Compare
        if(l != r)
            return (int)(l - r);    
    }

    return (int)(*s1 - *s2);
}

int strncasecmp(const char *s1, const char *s2, size_t n) {
    while(n) {
        // Convert Capital Letters to lowercase
        char l = __to_lower(*s1++);
        char r = __to_lower(*s2++);

        // Check for the end of the string
        if(!l || !r)
            return (int)(l - r);

        // Compare
        if(l != r)
            return (int)(l - r);        

        n--;
    }

    // Final Compare
    return (int)(*s1 - *s2);
}