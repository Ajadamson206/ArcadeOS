#include <serial.h>
#include <rawio.h>
#include <stdint.h>

/**
 * IO Port Offset 	    Setting of DLAB 	I/O Access 	Register mapped to this port
 * +0 	                0 	                Read 	    Receive buffer.
 * +0 	                0 	                Write 	    Transmit buffer.
 * +1 	                0 	                Read/Write 	Interrupt Enable Register.
 * +0 	                1 	                Read/Write 	With DLAB set to 1, this is the least significant byte of the divisor value for setting the baud rate.
 * +1 	                1 	                Read/Write 	With DLAB set to 1, this is the most significant byte of the divisor value.
 * +2 	                - 	                Read 	    Interrupt Identification
 * +2 	                - 	                Write 	    FIFO control registers
 * +3 	                - 	                Read/Write 	Line Control Register. The most significant bit of this register is the DLAB.
 * +4 	                - 	                Read/Write 	Modem Control Register.
 * +5 	                - 	                Read 	    Line Status Register.
 * +6 	                - 	                Read 	    Modem Status Register.
 * +7 	                - 	                Read/Write 	Scratch Register. 
 */

// Code adapted from https://wiki.osdev.org/Serial_Ports
int init_serial(serial_port port_num) {
    u16 port = (u16)port_num;

    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB
    outb(port + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
    outb(port + 1, 0x00);    //                  (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    if(inb(port + 0) != 0xAE) {
        return 1;
    }

   outb(port + 4, 0x0F);
   return 0;
}

// Code adapted from https://wiki.osdev.org/Serial_Ports
static int is_transmit_empty(serial_port port_num) {
    u16 port = (u16)port_num;
    return inb(port + 5) & 0x20;
}

// Code adapted from https://wiki.osdev.org/Serial_Ports
static void write_serial(serial_port port_num, char a) {
    u16 port = (u16)port_num;

    while (is_transmit_empty(port_num) == 0);
    
    outb(port,a);
}

void serial_print(serial_port port_num, const char* str) {
    while(*str) {
        write_serial(port_num, *str++);
    }
}
