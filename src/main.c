#include <stdbool.h>

#include "../include/display.h"
#include "../include/game.h"

int
main(void)
{
    WINDOW *board_win;
    game g;

    /* Initialization stuff */
    init_ncurses();
    init_game(&g);

    select_mode(&g);
    some_function(&g);

    /* This prints below the board, hence 16 */
    mvprintw(16, 0, "Thanks for playing! Press any key to exit.");
    refresh();
    getch();

    delwin(board_win);
    board_win = NULL;
    endwin();
    return 0;
}

/* EOF */
