#ifndef DISPLAY_H
#define DISPLAY_H
#include <ncurses.h>

/**
 * Initializes ncurses with select settings for the game
 */
void init_ncurses(void);

/**
 * Prints the current state of the board to the given window
 * @param win The window in which the board will be printed
 * @param board The board to be printed
 */
void print_board(WINDOW *win, const char *board);

#endif
/* EOF */
