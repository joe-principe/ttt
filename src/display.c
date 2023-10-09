#include "../include/display.h"
#include "../include/game.h"

/* Initializes the ncurses stuff */
void
init_ncurses(void)
{
    initscr();
    noecho();
    cbreak();
    refresh();
    curs_set(0);
}

/* Prints out the selection window */
void
print_menu(WINDOW *win, const char **text, int num_opts, int highlight)
{
    int i;

    for (i = 1; i < num_opts; i++) {
        if (i == highlight) { attron(A_STANDOUT); }
        mvwprintw(win, i, 1, "%s", text[i - 1]);
        attroff(A_STANDOUT);
    } /* for */
}

void 
navigate_menu(void (*funcp)(game *, int), game *g, int key, int num_opts,
              int *highlight)
{
    switch (key) {
        case KEY_UP:
        case KEY_LEFT:
        case 'k':
            *highlight -= 1;
            if (*highlight < 1) { *highlight = num_opts; }
            break;
        case KEY_DOWN:
        case KEY_RIGHT:
        case 'j':
            *highlight += 1;
            if (*highlight > num_opts) { *highlight = 1; }
            break;
        case 10:
            funcp(g, *highlight);
            break;
        default:
            break;
    } /* switch */
}

/* Prints the board to the window */
void
print_board(WINDOW *win, const char *board)
{
    mvwprintw(win, 0, 0, "     |   |   ");
    mvwprintw(win, 1, 0, "A  %c | %c | %c ", board[0], board[1], board[2]);
    mvwprintw(win, 2, 0, "  ___|___|___");
    mvwprintw(win, 3, 0, "     |   |   ");
    mvwprintw(win, 4, 0, "B  %c | %c | %c ", board[3], board[4], board[5]);
    mvwprintw(win, 5, 0, "  ___|___|___");
    mvwprintw(win, 6, 0, "     |   |   ");
    mvwprintw(win, 7, 0, "C  %c | %c | %c ", board[6], board[7], board[8]);
    mvwprintw(win, 8, 0, "     |   |   ");
    mvwprintw(win, 9, 0, "\n");
    mvwprintw(win, 10, 0, "   1 | 2 | 3 ");
    wrefresh(win);
}
/* EOF */
