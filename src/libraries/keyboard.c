#include <keyboard.h>
#include <stdint.h>
#include <rawio.h>

static const char kb_lookup_table[] = {
    27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 8, '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
    'I', 'O', 'P', '[', ']', '\n', 0, 'A', 'S', 'D', 'F',
    'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z',
    'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', 
    '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, 0
};

char kb_to_ascii(u16 kb_sc) {
    if((kb_sc > 0x58 && kb_sc < 0x81) || kb_sc > 0xD8)
        return 0;
    if(kb_sc >= 0x81)
        return kb_lookup_table[kb_sc - 0x80];
    else
        return kb_lookup_table[kb_sc];
}

u16 kb_wait_until_action(void) {
    while(!(inb(COMMAND_PORT) & 1));
    u16 code = inb(DATA_PORT);
    if(code == 0xE0) {
        code<<=8;
        code |= inb(DATA_PORT);
    }

    return code;
}