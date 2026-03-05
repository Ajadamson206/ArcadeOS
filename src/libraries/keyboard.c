#include <keyboard.h>
#include <stdint.h>
#include <rawio.h>

u8 kb_wait_until_action(void) {
    while(!(inb(COMMAND_PORT) & 1));
    return inb(DATA_PORT);
}