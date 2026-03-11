#pragma once

#include <stdint.h>

#define DATA_PORT 0x60
#define COMMAND_PORT 0x64

/* Keys Pressed */

#define KEY_ESC_PRESSED 0x01
#define KEY_1_PRESSED 0x02
#define KEY_2_PRESSED 0x03
#define KEY_3_PRESSED 0x04
#define KEY_4_PRESSED 0x05
#define KEY_5_PRESSED 0x06
#define KEY_6_PRESSED 0x07
#define KEY_7_PRESSED 0x08
#define KEY_8_PRESSED 0x09
#define KEY_9_PRESSED 0x0A
#define KEY_0_PRESSED 0x0B
#define KEY_MINUS_PRESSED 0x0C
#define KEY_EQUALS_PRESSED 0x0D
#define KEY_BACK_SPC_PRESSED 0x0E
#define KEY_TAB_PRESSED 0x0F
#define KEY_Q_PRESSED 0x10
#define KEY_W_PRESSED 0x11
#define KEY_E_PRESSED 0x12
#define KEY_R_PRESSED 0x13
#define KEY_T_PRESSED 0x14
#define KEY_Y_PRESSED 0x15
#define KEY_U_PRESSED 0x16
#define KEY_I_PRESSED 0x17
#define KEY_O_PRESSED 0x18
#define KEY_P_PRESSED 0x19
#define KEY_L_BRACKET_PRESSED 0x1A
#define KEY_R_BRACKET_PRESSED 0x1B
#define KEY_ENTER_PRESSED 0x1C
#define KEY_L_CTRL_PRESSED 0x1D
#define KEY_A_PRESSED 0x1E
#define KEY_S_PRESSED 0x1F
#define KEY_D_PRESSED 0x20
#define KEY_F_PRESSED 0x21
#define KEY_G_PRESSED 0x22
#define KEY_H_PRESSED 0x23
#define KEY_J_PRESSED 0x24
#define KEY_K_PRESSED 0x25 	
#define KEY_L_PRESSED 0x26 	
#define KEY_SEMI_COLON_PRESSED 0x27
#define KEY_SINGLE_QUOTE_PRESSED 0x28 	
#define KEY_BACK_TICK_PRESSED 0x29 	
#define KEY_L_SHIFT_PRESSED 0x2A 	
#define KEY_BACK_SLASH_PRESSED 0x2B
#define KEY_Z_PRESSED 0x2C 	
#define KEY_X_PRESSED 0x2D 	
#define KEY_C_PRESSED 0x2E 	
#define KEY_V_PRESSED 0x2F
#define KEY_B_PRESSED 0x30 	
#define KEY_N_PRESSED 0x31 	
#define KEY_M_PRESSED 0x32 	
#define KEY_COMMA_PRESSED 0x33
#define KEY_PERIOD_PRESSED 0x34 	
#define KEY_FRONT_SLASH_PRESSED 0x35 	
#define KEY_R_SHIFT_PRESSED 0x36 	
#define KEYPAD_STAR_PRESSED 0x37
#define KEY_L_ALT_PRESSED 0x38
#define KEY_SPACE_PRESSED 0x39
#define KEY_CAPS_LOCK_PRESSED 0x3A 	
#define KEY_F1_PRESSED 0x3B
#define KEY_F2_PRESSED 0x3C
#define KEY_F3_PRESSED 0x3D
#define KEY_F4_PRESSED 0x3E
#define KEY_F5_PRESSED 0x3F
#define KEY_F6_PRESSED 0x40
#define KEY_F7_PRESSED 0x41
#define KEY_F8_PRESSED 0x42
#define KEY_F9_PRESSED 0x43
#define KEY_F10_PRESSED 0x44
#define KEY_NUM_LOCK_PRESSED 0x45 	
#define KEY_SCRL_LOCK_PRESSED 0x46 	
#define KEYPAD_7_PRESSED 0x47
#define KEYPAD_8_PRESSED 0x48
#define KEYPAD_9_PRESSED 0x49
#define KEYPAD_MINUS_PRESSED 0x4A 	
#define KEYPAD_4_PRESSED 0x4B
#define KEYPAD_5_PRESSED 0x4C
#define KEYPAD_6_PRESSED 0x4D
#define KEYPAD_PLUS_PRESSED 0x4E 	
#define KEYPAD_1_PRESSED 0x4F
#define KEYPAD_2_PRESSED 0x50
#define KEYPAD_3_PRESSED 0x51
#define KEYPAD_0_PRESSED 0x52
#define KEYPAD_PERIOD_PRESSED 0x53
#define KEY_F11_PRESSED 0x57
#define KEY_F12_PRESSED 0x58

/* Extended Pressed */

#define KEY_ARROW_UP_PRESSED 0xE048
#define KEY_ARROW_LEFT_PRESSED 0xE04B
#define KEY_ARROW_RIGHT_PRESSED 0xE04D
#define KEY_ARROW_DOWN_PRESSED 0xE050

/* Released Keys */

#define KEY_ESC_RELEASED 0x81
#define KEY_1_RELEASED 0x82
#define KEY_2_RELEASED 0x83
#define KEY_3_RELEASED 0x84
#define KEY_4_RELEASED 0x85
#define KEY_5_RELEASED 0x86
#define KEY_6_RELEASED 0x87
#define KEY_7_RELEASED 0x88
#define KEY_8_RELEASED 0x89
#define KEY_9_RELEASED 0x8A
#define KEY_0_RELEASED 0x8B
#define KEY_MINUS_RELEASED 0x8C
#define KEY_EQUALS_RELEASED 0x8D
#define KEY_BACK_SPC_RELEASED 0x8E
#define KEY_TAB_RELEASED 0x8F
#define KEY_Q_RELEASED 0x90
#define KEY_W_RELEASED 0x91
#define KEY_E_RELEASED 0x92
#define KEY_R_RELEASED 0x93
#define KEY_T_RELEASED 0x94
#define KEY_Y_RELEASED 0x95
#define KEY_U_RELEASED 0x96
#define KEY_I_RELEASED 0x97
#define KEY_O_RELEASED 0x98
#define KEY_P_RELEASED 0x99
#define KEY_L_BRACKET_RELEASED 0x9A
#define KEY_R_BRACKET_RELEASED 0x9B
#define KEY_ENTER_RELEASED 0x9C
#define KEY_L_CTRL_RELEASED 0x9D
#define KEY_A_RELEASED 0x9E
#define KEY_S_RELEASED 0x9F
#define KEY_D_RELEASED 0xA0
#define KEY_F_RELEASED 0xA1
#define KEY_G_RELEASED 0xA2
#define KEY_H_RELEASED 0xA3
#define KEY_J_RELEASED 0xA4
#define KEY_K_RELEASED 0xA5
#define KEY_L_RELEASED 0xA6
#define KEY_SEMI_COLON_RELEASED 0xA7
#define KEY_SINGLE_QUOTE_RELEASED 0xA8
#define KEY_BACK_TICK_RELEASED 0xA9
#define KEY_L_SHIFT_RELEASED 0xAA
#define KEY_BACK_SLASH_RELEASED 0xAB
#define KEY_Z_RELEASED 0xAC
#define KEY_X_RELEASED 0xAD
#define KEY_C_RELEASED 0xAE
#define KEY_V_RELEASED 0xAF
#define KEY_B_RELEASED 0xB0
#define KEY_N_RELEASED 0xB1
#define KEY_M_RELEASED 0xB2
#define KEY_COMMA_RELEASED 0xB3
#define KEY_PERIOD_RELEASED 0xB4
#define KEY_FRONT_SLASH_RELEASED 0xB5
#define KEY_R_SHIFT_RELEASED 0xB6
#define KEYPAD_STAR_RELEASED 0xB7
#define KEY_L_ALT_RELEASED 0xB8
#define KEY_SPACE_RELEASED 0xB9
#define KEY_CAPS_LOCK_RELEASED 0xBA
#define KEY_F1_RELEASED 0xBB
#define KEY_F2_RELEASED 0xBC
#define KEY_F3_RELEASED 0xBD
#define KEY_F4_RELEASED 0xBE
#define KEY_F5_RELEASED 0xBF
#define KEY_F6_RELEASED 0xC0
#define KEY_F7_RELEASED 0xC1
#define KEY_F8_RELEASED 0xC2
#define KEY_F9_RELEASED 0xC3
#define KEY_F10_RELEASED 0xC4
#define KEY_NUM_LOCK_RELEASED 0xC5
#define KEY_SCRL_LOCK_RELEASED 0xC6
#define KEYPAD_7_RELEASED 0xC7
#define KEYPAD_8_RELEASED 0xC8
#define KEYPAD_9_RELEASED 0xC9
#define KEYPAD_MINUS_RELEASED 0xCA
#define KEYPAD_4_RELEASED 0xCB
#define KEYPAD_5_RELEASED 0xCC
#define KEYPAD_6_RELEASED 0xCD
#define KEYPAD_PLUS_RELEASED 0xCE
#define KEYPAD_1_RELEASED 0xCF
#define KEYPAD_2_RELEASED 0xD0
#define KEYPAD_3_RELEASED 0xD1
#define KEYPAD_0_RELEASED 0xD2
#define KEYPAD_PERIOD_RELEASED 0xD3
#define KEY_F11_RELEASED 0xD7
#define KEY_F12_RELEASED 0xD8

#define KEY_ARROW_UP_RELEASED 0xE0C8
#define KEY_ARROW_LEFT_RELEASED 0xE0CB
#define KEY_ARROW_RIGHT_RELEASED 0xE0CD
#define KEY_ARROW_DOWN_RELEASED 0xE0D0

/**
 * Halts all actions until the keyboard is pressed
 * @returns (u16) Keyboard Scancode
 */
extern u16 kb_wait_until_action(void);

/**
 * Converts a keyboard scancode to its ascii equivalent
 * @returns 0 if there was no equivalent
 * @returns (char) ASCII Equivalent (Capitalized if applicable)
 */
extern char kb_to_ascii(u16 kb_sc);

/**
 * Called by the interrupt handler for the keyboard. Used to 
 * store what keys were pressed / released.
 */
extern void kb_on_activation(u8 code);

/**
 * Clear the pressed keys bitmask.
 */
extern void kb_clear_press_buff(void);

/**
 * Check if the keyboard has been pressed
 * @returns 1: if the keyboard has been pressed
 * @returns 0: if the keyboard has not been pressed
 */
extern int kb_check_if_pressed(void);

/**
 * Get the first key to be pressed in the bitmask
 * @returns 0: if not key has been pressed
 * @returns (u16): The key that has been pressed (Ex. if 'A' was 
 * pressed KEY_A_PRESSED will be returned)
 */
extern u16 kb_get_next_pressed_key(void);

/**
 * Print the current key bitmasks to COM1
 */
extern void print_key_presses(void);

