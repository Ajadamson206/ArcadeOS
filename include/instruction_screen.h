#pragma once

/**
 * Create a generic instruction screen for a game.
 * @param game_name (const char *) String ending with \0 for the name of the game
 * @param instructions (char *) String containing the isntructions. NOTE: Uses '\\n' characters
 * to move to the next line on the screen. It will replace all '\\n' characters with '\\0', to render
 * properly. Text will also not render onto the next line if it extends past it, and will instead stop early
 * @returns 1: if the user wants to quit to the main menu
 * @returns 0: if the user wants to continue into the game
 */
extern int instr_screen_create(const char *game_name, char *instructions);

extern void instr_test(void);