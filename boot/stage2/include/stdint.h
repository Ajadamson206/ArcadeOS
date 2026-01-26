#pragma once

// 8 bit integers
typedef signed char     i8;
typedef unsigned char   u8;

// 16 bit integers
typedef signed short    i16;
typedef unsigned short  u16;

// 32 bit integers
typedef signed int    i32;
typedef unsigned int  u32;

// 64 bit integers (Still possible with 64 bit architecture)
typedef signed long long    i64;
typedef unsigned long long  u64;

// NULL
#define NULL ((void*)0) 

/**
 * Compile Time Checks to verify correctness
 */

_Static_assert(sizeof(u8) == 1, "u8 is not 1 byte");
_Static_assert(sizeof(u16) == 2, "u16 is not 2 bytes");
_Static_assert(sizeof(u32) == 4, "u8 is not 4 bytes");
_Static_assert(sizeof(u64) == 8, "u8 is not 8 bytes");

_Static_assert(sizeof(void*) == 4, "Pointers are not 4 bytes");