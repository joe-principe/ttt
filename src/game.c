#include <stdlib.h>

#include "../include/display.h"
#include "../include/game.h"

void
init_game(game *game)
{
    int i;

    game->curr_player = 1;
    game->turn = 1;
    game->mode = MODE_QUIT;
    for (i = 0; i < 9; i++) { game->board[i] = ' '; }
}

void 
select_mode(game *g)
{
    WINDOW *mode_win;
    int mode_hi, key;
    bool should_continue = true;
    const char *gamemodes_str[] = {
        "Player vs. CPU",
        "Player vs. Player (Local)",
        "Player vs. Player (Remote)",
        "Quit"
    };

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
        print_menu(mode_win, gamemodes_str, 4, mode_hi);
        key = wgetch(mode_win);
        if (key == 10) {
            delwin(mode_win);
            mode_win = NULL;
            clear();
            should_continue = false;
        } /* if */
        navigate_menu(set_mode, g, key, 4, &mode_hi);
    } /* while */
}

/* This should do some sort of setting up for the game modes */
void
set_mode(game *game, int mode)
{
    game->mode = mode;

    switch (mode) {
        case MODE_LOCAL_CPU:
            configure_cpu(game);
            break;
        case MODE_LOCAL_PVP:
            break;
        case MODE_REMOTE_PVP:
            configure_remote();
            break;
        case MODE_QUIT:
            endwin();
            exit(EXIT_SUCCESS);
    } /* switch */
}

/* Sets up the computer opponent for local CPU games */
void
configure_cpu(game *g)
{
    int key, p1_hi = 1;
    WINDOW *sel_win = newwin(4, 8, 2, 0);
    const char *players[] = {
        "Player",
        "Computer"
    };

    box(sel_win, 0, 0);
    wrefresh(sel_win);
    keypad(sel_win, true);

    printw("Choose who will be player 1 (X)");
    refresh();

    while (true) {
        print_menu(sel_win, players, 2, p1_hi);
        key = wgetch(sel_win);
        navigate_menu(set_local_player, g, key, 2, &p1_hi);
    } /* while */
}

void
set_local_player(game *g, int player_num)
{
    g->local_player = player_num == 1 ? 1 : 2;
}

/* This is the main loop of the game */
void some_function(game *g)
{
    int i = 0, pos_hi = 0, status = -1, action;
    WINDOW *board_win = newwin(11, 13, 3, 0);

    wrefresh(board_win);
    keypad(board_win, true);

    while (status == -1) {
        mvprintw(0, 0, "Player %d's turn (turn %d):", g->curr_player, g->turn);
        mvprintw(1, 0, "Press ENTER to select a square");
        refresh();

        print_board(board_win, g->board);

        for (i = 0; i < 9; i++) {
            if (i == pos_hi) { wattron(board_win, A_STANDOUT); }
            mvwprintw(board_win, 1 + 3 * (i / 3), 3 + 4 * (i % 3), "%c",
                      g->board[i]);
            wattroff(board_win, A_STANDOUT);
        } /* for */
        action = wgetch(board_win);

        switch (action) {
            case KEY_UP:
                pos_hi -= 3;
                if (pos_hi < 0) { pos_hi += 9; }
                break;
            case KEY_DOWN:
                pos_hi += 3;
                if (pos_hi > 8) { pos_hi -= 9; }
                break;
            case KEY_LEFT:
                pos_hi--;
                if (pos_hi == -1) { pos_hi = 8; }
                break;
            case KEY_RIGHT:
                pos_hi++;
                if (pos_hi == 9) { pos_hi = 0; }
                break;
            case 10:
                place_piece(g->board, pos_hi, g->curr_player);
                g->curr_player = g->curr_player == 1 ? 2 : 1;
                g->turn++;
                break;
            default:
                break;
        } /* switch */

        /* Check for victory */
        /* Can only win after the 6th turn, hence the check */
        if (g->turn >= 6) { status = check_for_win(g->board, g->turn); }
    } /* while */

    print_board(board_win, g->board);
    switch (status) {
        case 0:
            mvprintw(15, 0, "The game is a tie!");
            break;
        case 1:
            mvprintw(15, 0, "Player 1 (X) wins!");
            break;
        case 2:
            mvprintw(15, 0, "Player 2 (O) wins");
            break;
        default:
            break;
    } /* switch */
}

/* Places a piece and increments the turn counter
 * If the current space is occupied, do nothing */
void
place_piece(char *board, int pos, int player)
{
    if (board[pos] != ' ') { return; }

    switch (player) {
        case 1:
            board[pos] = 'X';
            break;
        case 2:
            board[pos] = 'O';
            break;
        default:
            break;
    } /* switch */
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
