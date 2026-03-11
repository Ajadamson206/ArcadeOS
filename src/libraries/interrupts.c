#include <interrupts.h>
#include <graphics.h>
#include <serial.h>
#include <keyboard.h>
#include <rawio.h>

static idt_entry idt[IDT_COUNT];
static idtr idt_reg;

// Interrupt Stubs defined in interrupt_stub.asm
// Define during the linking
extern void isr0(void);
extern void isr6(void);
extern void isr8(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr32(void);
extern void isr33(void);

void idt_set_gate(u8 index, u32 handler, u16 selector, u8 type_attr) {
    idt[index].offset_low  = (u16)(handler & 0xFFFF);
    idt[index].selector    = selector;
    idt[index].zero        = 0;
    idt[index].type_attr   = type_attr;
    idt[index].offset_high = (u16)((handler >> 16) & 0xFFFF);
}

void idt_init(void) {
    // Set everything to zero in case I messed up with the .bss memory
    for(u32 i = 0; i < IDT_COUNT; i++) {
        idt_entry *idt_e = idt + i;
        idt_e->offset_high = 0;
        idt_e->offset_low = 0;
        idt_e->selector = 0;
        idt_e->type_attr = 0;
        idt_e->zero = 0;
    }

    // Set the gate to the stub

    // Errors
    idt_set_gate(IDT_DIV_ZERO, (u32)isr0, 0x08, IDT_ENTRY_P_BIT | INTRPT_GATE_32);
    idt_set_gate(IDT_INVALID_OPCODE, (u32)isr6, 0x08, IDT_ENTRY_P_BIT | INTRPT_GATE_32);
    idt_set_gate(IDT_DOUBLE_FAULT, (u32)isr8, 0x08, IDT_ENTRY_P_BIT | INTRPT_GATE_32);
    idt_set_gate(IDT_STACK_FAULT, (u32)isr12, 0x08, IDT_ENTRY_P_BIT | INTRPT_GATE_32);
    idt_set_gate(IDT_GENERAL_PROTECTION, (u32)isr13, 0x08, IDT_ENTRY_P_BIT | INTRPT_GATE_32);
    idt_set_gate(IDT_PAGE_FAULT, (u32)isr14, 0x08, IDT_ENTRY_P_BIT | INTRPT_GATE_32);

    // Misc 
    idt_set_gate(IDT_TIMER, (u32)isr32, 0x08, IDT_ENTRY_P_BIT | INTRPT_GATE_32);
    idt_set_gate(IDT_KEYBOARD, (u32)isr33, 0x08, IDT_ENTRY_P_BIT | INTRPT_GATE_32);


    // Update the IDTR
    idt_reg.base = (u32)idt;
    idt_reg.limit = sizeof(idt) - 1;

    // Load the IDT
    idt_load(&idt_reg);
}

void interrupt_dispatch(interrupt_frame *frame) {
    switch(frame->interrupt) {
        case IDT_DIV_ZERO:
        case IDT_INVALID_OPCODE:
        case IDT_STACK_FAULT:
        case IDT_GENERAL_PROTECTION:
        case IDT_PAGE_FAULT:
            interrupt_default_handler(frame->interrupt, frame->error);
            break;

        case IDT_TIMER:
            interrupt_timer_handler();
            break;
        case IDT_KEYBOARD:
            interrupt_keyboard_handler();
            break;

        default:
            interrupt_default_handler(frame->interrupt, frame->error);
    }
}

void interrupt_default_handler(u32 interrupt, u32 error) {
    fill_screen(interrupt);

    serial_print(COM1, "Interrupt Occurred: ");
    serial_print_hex(COM1, interrupt);
    serial_print(COM1, "\nError: ");
    serial_print_hex(COM1, error);

    __asm__ volatile ("cli");
    for (;;)
        __asm__ volatile ("hlt");
}

void interrupt_keyboard_handler(void) {
    // serial_print(COM1, "Keyboard Activated: ");

    u8 code = inb(DATA_PORT);

    kb_on_activation(code);

    // serial_print_hex(COM1, (u32)code);

    PIC_sendEOI(1);
}

void interrupt_timer_handler(void) {
    ticks++;

    PIC_sendEOI(0);
}

u64 get_pit_ticks(void) {
    return (u64)ticks;
}

void PIC_init(void) {
    // Set the Timer interrupt to be the start of PIC
    PIC_remap(IDT_TIMER, IDT_TIMER + 8);

    // Set the mask to ignore everything except timer + keyboard
    IRQ_replace_mask(0xFFFC);

    // Set configurations for the PIT (Programmable Interval Timer)
    PIT_init();
}

void PIC_remap(u32 offset1, u32 offset2) {
    // starts the initialization sequence (in cascade mode)
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

    // ICW2: Master PIC vector offset
	outb(PIC1_DATA, offset1);
	io_wait();

    // ICW2: Slave PIC vector offset
	outb(PIC2_DATA, offset2);
	io_wait();

    // ICW3: tell Master PIC that there is a slave PIC at IRQ2
	outb(PIC1_DATA, 1 << CASCADE_IRQ);
	io_wait();

    // ICW3: tell Slave PIC its cascade identity (0000 0010)
	outb(PIC2_DATA, 0x02);
	io_wait();
	
    // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICs.
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);
}

void IRQ_set_mask(u8 IRQline) {
    if(IRQline >= 16)
        return;
    
    u16 port;
    u8 value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}

void IRQ_clear_mask(u8 IRQline) {
    if(IRQline >= 16)
        return;
    
    u16 port;
    u8 value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}

void IRQ_replace_mask(u16 mask) {
    // Set PIC1
    outb(PIC1_DATA, (u8)(mask & 0x00FF));

    // Set PIC2
    outb(PIC2_DATA, (u8)(mask>>8));
}

u16 IRQ_get_mask(void) {
    return ((u16)inb(PIC2_DATA))<<8 | inb(PIC1_DATA);
}

void PIT_init(void) {
    u16 interrupt_frequency = PIT_INPUT_CLOCK_SPEED / PIT_FREQ;

    outb(0x43, 0x36);

    outb(0x40, interrupt_frequency & 0xFF);
    outb(0x40, interrupt_frequency >> 8);
}