#pragma once

#include <stdint.h>
#include <rawio.h>

struct __idt_entry {
    u16 offset_low;    // handler address bits 0..15
    u16 selector;      // code segment selector in GDT
    u8 zero;           // must be 0
    u8 type_attr;      // gate type + privilege + present
    u16 offset_high;   // handler address bits 16..31
} __attribute__((packed));

_Static_assert(sizeof(struct __idt_entry) == 8, "The __idt_entry struct does not equal 8 Bytes");

/**
 * An entry inside of the interrupt descriptor table. Contains all
 * of the information a particular entry would need.
 */
typedef struct __idt_entry idt_entry;

struct __idtr {
    u16 limit;
    u32 base;
} __attribute__((packed));

_Static_assert(sizeof(struct __idtr) == 6, "The __idtr struct does not equal 6 Bytes");

/**
 * Interrupt Descriptor Table Register:
 * 
 * Describes where the IDT is kept
 */
typedef struct __idtr idtr;

/* Describes the type of gate represented by a interrupt descriptor */

// Ignore
#define TASK_GATE 0x5

// For 16-bit protected mode handlers, clears the Interrupt Flag
#define INTRPT_GATE_16 0x6

// For 16-bit protected mode handlers, The Interrupt Flag is NOT cleared
#define TRAP_GATE_16 0x7

// For 32-bit protected mode handlers, clears the Interrupt Flag
#define INTRPT_GATE_32 0xE

// For 32-bit protected mode handlers, The Interrupt Flag is NOT cleared
#define TRAP_GATE_32 0xF

// Present Bit used in the type_attr value
#define IDT_ENTRY_P_BIT 0x80

/* Important Interrupts that I should make entries for */

#define IDT_DIV_ZERO 0

#define IDT_INVALID_OPCODE 6

// Called when an instruction generates an exception: Pushes Error of 0
#define IDT_DOUBLE_FAULT 8

// Error with the stack: Pushes Error Codes Automatically
#define IDT_STACK_FAULT 12

// Pushes Error Codes Automatically
#define IDT_GENERAL_PROTECTION 13

// Error Using Page Tables (Since it is a flat memory model this shouldn't be an issue)
// Pushes Error Code Automatically
#define IDT_PAGE_FAULT 14

/* Suggested Interrupts to make my life easier */

#define IDT_TIMER 32
#define IDT_KEYBOARD 33



/**
 * Basic Interrupt Frame
 * 
 * Allows me to see all registers and really see what went wrong
 */
typedef struct {
    u32 gs, fs, es, ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // General Registers
    u32 interrupt; // Interrupt Number
    u32 error; // Interrupt Error
    u32 eip, cs, eflags; // Processor Flags
} interrupt_frame;

// Number of potential entries in the Interrupt Descriptor Table
#define IDT_COUNT 256


#define PIC1		0x20		// IO base address for master PIC
#define PIC2		0xA0		// IO base address for slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20		// End-of-interrupt command code

#define ICW1_ICW4	0x01		// Indicates that ICW4 will be present
#define ICW1_SINGLE	0x02		// Single (cascade) mode
#define ICW1_INTERVAL4	0x04		// Call address interval 4 (8)
#define ICW1_LEVEL	0x08		// Level triggered (edge) mode
#define ICW1_INIT	0x10		// Initialization - required!

#define ICW4_8086	0x01		// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO	0x02		// Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08		// Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C		// Buffered mode/master
#define ICW4_SFNM	0x10		// Special fully nested (not)

#define CASCADE_IRQ 2

/**
 * Initialize the Programmable Interrupt Controller and the IRQ
 */
extern void PIC_init(void);

/**
 * Needed at the end of every interrupt function triggered by the PIC
 */
static inline void PIC_sendEOI(u8 irq) {
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
	
	outb(PIC1_COMMAND,PIC_EOI);
}


/**
 * Remap the 8259 Programmable Interrupt Controller (PIC)
 * @param offset1 (u32) vector offset for master PIC
 * @param offset2 (u32) vector offset for slave PIC
 */
extern void PIC_remap(u32 offset1, u32 offset2);

/**
 * Set a bit on the PIC's bitmap.
 * Note: When a bit is set, the PIC ignores the request and continues normal operation
 * 
 * @param IRQline (u8) What bit should be set [0-15]. Any digit > 15 will be ignored.
 * Also 0 maps to interrupt 32, so 15 maps to interrupt 47.
 */
extern void IRQ_set_mask(u8 IRQline);

/**
 * Clear a bit on the PIC's bitmap
 * @param IRQline (u8) What bit should be cleared [0-15]. Any digit > 15 will be ignored.
 */
extern void IRQ_clear_mask(u8 IRQline);

/**
 * Replace the entire PIC's bitmap with the passed in one
 * @param mask (u16) A bitmask defined by the user (Bit 0 -> Bit 0 of PIC)
 */
extern void IRQ_replace_mask(u16 mask);

/**
 * Get a copy of the PIC's current bitmask
 * @return (u16) bitmask of the PIC's (Upper 8 bits come from the Slave Chip)
 */
extern u16 IRQ_get_mask(void);

// Clock speed of the PIT
#define PIT_INPUT_CLOCK_SPEED 1193182U

// How often the PIT should trigger (1000 Hz -> 1 ms)
#define PIT_FREQ 1000U

#define PIT_COMMAND_REQ 0x43
#define PIT_CHANNEL_0_DATA 0x40

// Microseconds per Tick
#define US_PER_TICK (1000000ULL / PIT_FREQ)

/**
 * Initialize the Programmable Interval Timer
 */
extern void PIT_init(void);

// Function used to set an entry in the IDT
extern void idt_set_gate(u8 index, u32 handler, u16 selector, u8 type_attr);

static inline void idt_load(const idtr *idtr_ptr) {
    __asm__ volatile (
        "lidt [%0]" 
        : 
        : "r"(idtr_ptr) 
        : "memory"
    );
}

/**
 * Initialize the Interrupt Descriptor Table. To be called by the Kernel
 * upon startup.
 */
extern void idt_init(void);

/**
 * Interrupt Dispatcher. Decides which function to call for the chosen interrupt
 */
extern void interrupt_dispatch(interrupt_frame *frame);

/**
 * Interrupt Default Handler. Generic Handler that is used if the interrupt doesn't
 * have a predefined function
 */
extern void interrupt_default_handler(u32 interrupt, u32 error);

/**
 * IDT Function for keyboard input
 */
extern void interrupt_keyboard_handler(void);

/**
 * IDT Function for handling the timer
 */
extern void interrupt_timer_handler(void);

/**
 * Get the number of PIT ticks
 */
extern volatile u64 get_pit_ticks(void);

/**
 * Get the PIT timer uptime in milliseconds
 */
extern volatile u64 get_pit_timer_uptime(void);