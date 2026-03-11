#include <misc.h>
#include <stdint.h>
#include <interrupts.h>

void sleep(u64 milliseconds) {
    u64 new_time = ticks + milliseconds;
    while(new_time > ticks) {
        __asm__ volatile ("hlt");
    }
}