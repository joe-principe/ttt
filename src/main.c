#include <stdbool.h>

#include "../include/display.h"
#include "../include/game.h"

int
main(void)
{
    game g;

    /* Initialization stuff */
    init_ncurses();
    init_game(&g);

    select_mode(&g);
    game_loop(&g);

    /* This prints below the board, hence 16 */
    mvprintw(16, 0, "Thanks for playing! Press any key to exit.");
    refresh();
    getch();

    endwin();
    return 0;
}

/* EOF */
