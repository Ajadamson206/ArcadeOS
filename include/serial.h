#pragma once

/**
 * Code for interacting with serial ports
 * 
 * QEMU Uses Serial Ports as a stdio
 */

#include <stdint.h>

/* I/O Ports */

typedef enum {
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8,
    COM5 = 0x5F8,
    COM6 = 0x4F8,
    COM7 = 0x5E8,
    COM8 = 0x4E8 
} serial_port ;

/**
 * Initialize a serial port for use
 * 
 * @param port_num (serial_port) COM1-COM8
 * @returns 1 - Successfully Initialized
 * 
 * 0 - Failed to Initialize Port
 */
extern int init_serial(serial_port port_num);

/**
 * Write a string to a serial port
 * 
 * @param port_num (serial_port) A serial port (COM1-COM8) 
 * that has already been initialized
 * @param str (const char*) The string to be written
 */
extern void serial_print(serial_port port_num, const char* str);


/**
 * Output an ASCII hexadecimal number to the a serial port
 * 
 * @param port_num (serial_port) A serial port (COM1-COM8) that has already
 * been initialized
 * @param hex (u32) The hex to be outputted and converted to ASCII
 */
extern void serial_print_hex(serial_port port_num, u32 hex);


