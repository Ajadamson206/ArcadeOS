#pragma once

/**
 * This file provides a couple of typedefs and macros that will assist with development
 * Most of them just specify the size of the variable to make it more clear
 * since this is for a 32-bit architecture 
 */

/***
 * Assume:
 *  - char      = 1 Byte
 *  - short     = 2 Bytes
 *  - int       = 4 Bytes
 *  - long long = 8 Bytes
 */

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

/**
 * Limits
 */

#define I8_MAX ((i8)0x7F)
#define I16_MAX ((i16)0x7FFF)
#define I32_MAX ((i32)0x7FFFFFFF)
#define I64_MAX ((i64)0x7FFFFFFFFFFFFFFFLL)

#define I8_MIN ((i8)(-I8_MAX - 1))
#define I16_MIN ((i16)(-I16_MAX - 1))
#define I32_MIN ((i32)(-I32_MAX - 1))
#define I64_MIN ((i64)(-I64_MAX - 1))

#define U8_MIN ((u8)0)
#define U16_MIN ((u16)0)
#define U32_MIN ((u32)0)
#define U64_MIN ((u64)0)

#define U8_MAX ((u8)0xFFu)
#define U16_MAX ((u16)0xFFFFu)
#define U32_MAX ((u32)0xFFFFFFFFu)
#define U64_MAX ((u64)0xFFFFFFFFFFFFFFFFULL)

/**
 * Compile Time Checks to verify correctness
 */

_Static_assert(sizeof(u8) == 1, "u8 is not 1 byte");
_Static_assert(sizeof(u16) == 2, "u16 is not 2 bytes");
_Static_assert(sizeof(u32) == 4, "u8 is not 4 bytes");
_Static_assert(sizeof(u64) == 8, "u8 is not 8 bytes");

_Static_assert(sizeof(void*) == 4, "Pointers are not 4 bytes");
