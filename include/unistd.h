#pragma once

#include <stdint.h>

extern void sleep_ticks(u64 num_ticks);
extern void sleep(unsigned int seconds);
extern void usleep(u64 usec);