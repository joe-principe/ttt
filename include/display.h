#ifndef DISPLAY_H
#define DISPLAY_H
#include <ncurses.h>

#include "../include/game.h"

/**
 * Initializes ncurses with select settings for the game
 */
void init_ncurses(void);

/**
 * Prints the selection menu window options
 * @param win The selection menu window
 * @param num_opts The number of options the user can select
 * @param highlight The currently highlighted option
 * @param text The text options
 */
void print_menu(WINDOW *win, const char **text, int num_opts, int highlight);

/**
 * Allows the user to navigate selection menu
 * @param funcp The function to be executed upon selection
 * @param game The game to set stuff for
 * @param key The key the user pressed
 * @param num_opts The number of options
 * @param highlight The currently highlighted option
 */
void navigate_menu(void (*funcp)(game *g, int n), game *g, int key, int num_opts, int *highlight);

/**
 * Prints the current state of the board to the given window
 * @param win The window in which the board will be printed
 * @param board The board to be printed
 */
void print_board(WINDOW *win, const char *board);

#endif
/* EOF */
