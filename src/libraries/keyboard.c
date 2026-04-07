#include <keyboard.h>
#include <stdint.h>
#include <rawio.h>
#include <serial.h>

static void (*kb_hook)(u16);

static const char kb_lookup_table[] = {
    27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 8, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u',
    'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f',
    'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
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

// Store if a key has been pressed
volatile int kb_has_been_pressed = 0;

// Stores the arrow keys
volatile u8 kb_misc_codes = 0;

// Stores regular keyboard presses
volatile u64 kb_reg_codes[2] = {0};

// Used for the extended codes. Keyboard only sends 1 Byte at a time
// The extended codes are 2 Bytes
static volatile u8 kb_extended_prefix = 0;

static inline void update_has_been_pressed(void){
    kb_has_been_pressed = !((kb_reg_codes[0] == 0) && (kb_reg_codes[1] == 0) && (kb_misc_codes == 0));
}

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
    if (code == 0xE0) {
        kb_extended_prefix = 1;
        return;
    }

    if (kb_extended_prefix) {
        u16 ext_code = 0xE000U | code;
        kb_extended_prefix = 0;

        kb_extended_activation(ext_code);
        update_has_been_pressed();

        return;
    }

    // Press Codes are bounded from 0x00 - 0x58
    // Release Codes are bounded from 0x81 - 0xD8
    if (code <= 0x58) { // Press Codes
        kb_reg_codes[code / 64] |= (1ULL<<(code % 64));
        kb_has_been_pressed = 1;
    } else if (code >= 0x81 && code <= 0xD8) { // Release Codes
        u8 updated_code = code - 0x80;
        kb_reg_codes[updated_code / 64] &= 0xFFFFFFFFFFFFFFFFULL ^ 1ULL<<(updated_code % 64);
        
        // I could probably get away with just an XOR but if a release code ever gets sent
        // without a previous press code it could cause some issues

        // Every release check if a key is still being pressed
        update_has_been_pressed();
    }

}

void kb_clear_press_buff(void) {
    kb_reg_codes[0] = 0;
    kb_reg_codes[1] = 0;
    kb_misc_codes = 0;

    // Just in case an interrupt happened in the middle of executing this function
    update_has_been_pressed();
}

int kb_check_if_pressed(void) {
    return kb_has_been_pressed; 
}

static const u16 arrow_key_map[] = {KEY_ARROW_UP_PRESSED, KEY_ARROW_LEFT_PRESSED, KEY_ARROW_RIGHT_PRESSED, KEY_ARROW_DOWN_PRESSED, 0, 0, 0, 0 }; 

u16 kb_get_next_pressed_key(void) {
    if(!kb_has_been_pressed)
        return 0x0000;

    if(kb_misc_codes) {
        u8 index = 0;
        while(index < 8) {
            if(kb_misc_codes & (1<<index)) {
                kb_misc_codes &= 0xFF ^ (1<<index);
                update_has_been_pressed();
                return arrow_key_map[index];
            }
            index++;
        }

        return 0;
    }

    if(kb_reg_codes[0]) {
        u16 index = 0;
        while(index < 64) {
            if(kb_reg_codes[0] & (1ULL<<index)) {
                kb_reg_codes[0] &= 0xFFFFFFFFFFFFFFFFULL ^ 1ULL<<index;
                update_has_been_pressed();
                return index;
            }
            index++;
        }

        return 0;
    }

        if(kb_reg_codes[1]) {
            u16 index = 0;
            while(index < 64) {
                if(kb_reg_codes[1] & (1ULL<<index)) {
                    kb_reg_codes[1] &= 0xFFFFFFFFFFFFFFFFULL ^ 1ULL<<index;
                    update_has_been_pressed();
                    return index;
                }
                index++;
        }

        return 0;
    }

    return 0;
}

void print_key_presses(void) {
    serial_print(COM1, "Arrow Keys: ");
    serial_print_hex(COM1, (u32)kb_misc_codes);
    serial_print(COM1, "\nReg Codes 0: ");
    serial_print_hex(COM1, (u32)((kb_reg_codes[0])>>32));
    serial_print_hex(COM1, (u32)((kb_reg_codes[0])));
    serial_print(COM1, "\nReg Codes 1: ");
    serial_print_hex(COM1, (u32)((kb_reg_codes[1])>>32));
    serial_print_hex(COM1, (u32)((kb_reg_codes[1])));
}

void kb_hook_add(void (*kb_hook_func)(u16)) {
    kb_hook = kb_hook_func;
}

void kb_hook_remove(void) {
    kb_hook = NULL;
}