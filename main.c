#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

int choose_gamemode(int, const char **);

int
main(void)
{
    int i;
    int highlight = 0, choice = 0;
    int mode_choice = 0;
    bool should_continue = true;
    WINDOW *mode_win;
    const char *gamemodes[] = {
        "Player vs. CPU",
        "Player vs. Player (Local)",
        "Player vs. Player (Remote)",
        "Quit"
    };

    /* ncurses initialization stuff */
    initscr();
    noecho();
    cbreak();
    refresh();
    curs_set(0);

    /* Setup the selection window */
    mode_win = newwin(6, 28, 3, 0);
    box(mode_win, 0, 0);
    wrefresh(mode_win);
    keypad(mode_win, true);

    printw("Welcome to Tic-Tac-Toe!");
    mvprintw(1, 0, "Choose a gamemode:");
    refresh();

    while (should_continue) {
        for (i = 0; i < 4; i++) {
            if (i == highlight) { wattron(mode_win, A_REVERSE); }
            mvwprintw(mode_win, i + 1, 1, "%s", gamemodes[i]);
            wattroff(mode_win, A_REVERSE);
        } /* for */
        choice = wgetch(mode_win);

        switch (choice) {
            case KEY_UP:
                highlight--;
                if (highlight == -1) { highlight = 3; }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight == 4) { highlight = 0; }
                break;
            /* TODO: Figure out why 10 != KEY_ENTER */
            case 10:
                mode_choice = choose_gamemode(highlight, gamemodes);
                break;
            default:
                break;
        } /* switch */
        should_continue = mode_choice != -1 ? 1 : 0;
    } /* while */

    delwin(mode_win);
    endwin();
    return 0;
}

/* Make this so it chooses the gamemode and whether or not to quit in a logical
 * manner. Rather than returning game mode, it returns whether or not to quit */
int
choose_gamemode(int sel, const char **gamemodes)
{
    int choice;
    switch (sel) {
        case 0:
            /* Start player vs cpu mode */
            move(9, 0);
            clrtoeol();
            printw("You chose %s", gamemodes[sel]);
            choice = 0;
            break;
        case 1:
            /* Start local pvp mode */
            move(9, 0);
            clrtoeol();
            printw("You chose %s", gamemodes[sel]);
            choice = 1;
            break;
        case 2:
            /* Start remote pvp mode */
            move(9, 0);
            clrtoeol();
            printw("You chose %s", gamemodes[sel]);
            choice = 2;
            break;
        default:
            choice = -1;
            break;
    } /* switch */
    refresh();

    return choice;
}
/* EOF */
