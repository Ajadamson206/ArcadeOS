#include <keyboard.h>
#include <stdint.h>
#include <rawio.h>
#include <serial.h>

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

// Stores the arrow keys
u8 kb_misc_codes = 0;

// Stores regular keyboard presses
u64 kb_reg_codes[2] = {0};

// Used for the extended codes. Keyboard only sends 1 Byte at a time
// The extended codes are 2 Bytes
static u8 kb_previous_code = 0;

static void kb_extended_activation(u16 extended_code) {
    switch (extended_code)
    {
        // Press Codes
        case KEY_ARROW_UP_PRESSED:
            kb_misc_codes |= 1<<0;
            break;
        case KEY_ARROW_LEFT_PRESSED:
            kb_misc_codes |= 1<<1;
            break;
        case KEY_ARROW_RIGHT_PRESSED:
            kb_misc_codes |= 1<<2;
            break;
        case KEY_ARROW_DOWN_PRESSED:
            kb_misc_codes |= 1<<3;
            break;
        
        // Release Codes
        case KEY_ARROW_UP_RELEASED:
            kb_misc_codes &= 0xFF ^ (1<<0);
            break;
        case KEY_ARROW_LEFT_RELEASED:
            kb_misc_codes &= 0xFF ^ (1<<1);
            break;
        case KEY_ARROW_RIGHT_RELEASED:
            kb_misc_codes &= 0xFF ^ (1<<2);
            break;
        case KEY_ARROW_DOWN_RELEASED:
            kb_misc_codes &= 0xFF ^ (1<<3);
            break;
    
        default:
            break;
    }
}

void kb_on_activation(u8 code) {
    // Store what keys were pressed into a bitmask
    // The released codes unset the particular bits
    
    // Extended Codes
    if (kb_previous_code == 0xE0 || kb_previous_code == 0xE1) {
        u16 ext_code = (u16)(kb_previous_code<<8) | code;
        kb_extended_activation(code);
    }

    // Press Codes are bounded from 0x00 - 0x58
    // Release Codes are bounded from 0x81 - 0xD8
    if (code <= 0x58) { // Press Codes
        kb_reg_codes[code / 64] |= 1<<(code % 64);
    } else if (code > 0x81 && code < 0xD8) { // Release Codes
        u8 updated_code = code - 0x81;
        kb_reg_codes[updated_code / 64] &= 0xFFFFFFFFFFFFFFFFULL ^ 1ULL<<(updated_code % 64);
        
        // I could probably get away with just an XOR but if a release code ever gets sent
        // without a previous press code it could cause some issues
    }

    kb_previous_code = code;
}

