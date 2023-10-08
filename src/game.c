#include <ncurses.h>
#include <stdlib.h>

#include "../include/game.h"

const char *gamemodes_str[] = {
    "Player vs. CPU",
    "Player vs. Player (Local)",
    "Player vs. Player (Remote)",
    "Quit"
};

void
init_game(game *game)
{
    int i;

    game->curr_player = 1;
    game->turn = 1;
    game->mode = MODE_QUIT;
    for (i = 0; i < 9; i++) { game->board[i] = ' '; }
}

/* This should do some sort of setting up for the game modes */
void
set_mode(int mode)
{
    switch (mode) {
        case MODE_LOCAL_CPU:
        case MODE_LOCAL_PVP:
        case MODE_REMOTE_PVP:
            return;
        case MODE_QUIT:
            endwin();
            exit(EXIT_SUCCESS);
    } /* switch */
}

void 
select_mode(void)
{
    WINDOW *mode_win;
    int i, mode_hi = 0, action = 0;
    bool should_continue = true;

    /* Setup the gamemode selection window */
    mode_win = newwin(6, 28, 3, 0);
    box(mode_win, 0, 0);
    wrefresh(mode_win);
    keypad(mode_win, true);

    printw("Welcome to Tic-Tac-Toe!");
    mvprintw(1, 0, "Choose a gamemode:");
    refresh();

    /* Choose game mode and start game */
    while (should_continue) {
        for (i = 0; i < 4; i++) {
            if (i == mode_hi) { wattron(mode_win, A_STANDOUT); }
            mvwprintw(mode_win, i + 1, 1, "%s", gamemodes_str[i]);
            wattroff(mode_win, A_STANDOUT);
        } /* for */
        action = wgetch(mode_win);

        switch (action) {
            case KEY_UP:
            case KEY_LEFT:
            case 'k':
                mode_hi--;
                if (mode_hi == -1) { mode_hi = 3; }
                break;
            case KEY_DOWN:
            case KEY_RIGHT:
            case 'j':
                mode_hi++;
                if (mode_hi == 4) { mode_hi = 0; }
                break;
            /* TODO: Figure out why 10 != KEY_ENTER */
            case 10:
                delwin(mode_win);
                mode_win = NULL;
                clear();
                set_mode(mode_hi);
                should_continue = false;
                break;
            default:
                break;
        } /* switch */
    } /* while */
}

/* Places a piece and increments the turn counter
 * If the current space is occupied, do nothing */
void
place_piece(char *board, int pos, int *player, int *turn)
{
    if (board[pos] != ' ') { return; }

    switch (*player) {
        case 1:
            board[pos] = 'X';
            *player = 2;
            break;
        case 2:
            board[pos] = 'O';
            *player = 1;
            break;
        default:
            break;
    } /* switch */
    *turn += 1;
}

/* Checks the status of the board */
int
check_for_win(const char *board, int turn)
{
    int i;

    /* Check the rows for a victor */
    for (i = 0; i < 9; i += 3) {
        if (board[i] == board[i + 1] && board[i + 1] == board[i + 2]) {
            if (board[i] == 'X') { return 1; }
            else if (board[i] == 'O') { return 2; }
        } /* if */
    } /* for */

    /* Check the columns for a victor */
    for (i = 0; i < 3; i++) {
        if (board[i] == board[i + 3] && board[i + 3] == board[i + 6]) {
            if (board[i] == 'X') { return 1; }
            else if (board[i] == 'O') { return 2; }
        } /* if */
    } /* for */

    /* Checks the backslash diagonal */
    if (board[0] == board[4] && board[4] == board[8]) {
        if (board[0] == 'X') { return 1; }
        else if (board[0] == 'O') { return 2; }
    } /* if */

    /* Checks the forwardslash diagonal */
    if (board[6] == board[4] && board[4] == board[2]) {
        if (board[0] == 'X') { return 1; }
        else if (board[0] == 'O') { return 2; }
    } /* if */

    /* Checks if the board is full with no winner */
    if (turn == 10) { return 0; }

    /* Continues the game */
    return -1;
}
/* EOF */
