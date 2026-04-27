#include <stdint.h>
#include <interrupts.h>
#include <serial.h>
#include <64bitoperations.h>


void sleep_ticks(u64 num_ticks) {
    volatile u64 end = get_pit_ticks() + num_ticks;

    for (;;) {
        // Prevent the timer from firing immediately after the check
        __asm__ volatile ("cli");

        volatile u64 ticks = get_pit_ticks();
        if ((i64)(end - ticks) <= 0) {
            __asm__ volatile ("sti");
            break;
        }

        __asm__ volatile (
            "sti\n"
            "hlt"
        );
    }
}

void sleep(unsigned int seconds) {
    u64 ticks = (u64)seconds * PIT_FREQ;
    sleep_ticks(ticks);
}

void usleep(u64 usec) {
    u64 ticks = (usec + US_PER_TICK - 1) / US_PER_TICK; // ceil division
    
    if (ticks == 0) 
        ticks = 1;
    
    sleep_ticks(ticks);
}