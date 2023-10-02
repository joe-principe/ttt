#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

enum gamemodes {
    MODE_LOCAL_CPU = 0,
    MODE_LOCAL_PVP = 1,
    MODE_REMOTE_PVP = 2,
    MODE_QUIT = -1
};

void init_ncurses(void);
void init_game(int);
void print_board(WINDOW *, const char *);
void place_piece(int *, char *, int, int *);
int check_for_win(const char *, int);

int
main(void)
{
    int i;
    int highlight = 0, opt = 0;
    int curr_player = 1;
    int turn = 1;
    int status = -1;
    char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    bool should_continue = true;
    const char *gamemodes_str[] = {
        "Player vs. CPU",
        "Player vs. Player (Local)",
        "Player vs. Player (Remote)",
        "Quit"
    };
    WINDOW *mode_win, *board_win;

    /* ncurses initialization stuff */
    init_ncurses();

    /* Setup the selection window */
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
            if (i == highlight) { wattron(mode_win, A_STANDOUT); }
            mvwprintw(mode_win, i + 1, 1, "%s", gamemodes_str[i]);
            wattroff(mode_win, A_STANDOUT);
        } /* for */
        opt = wgetch(mode_win);

        switch (opt) {
            case KEY_UP:
            case KEY_LEFT:
            case 'k':
                highlight--;
                if (highlight == -1) { highlight = 3; }
                break;
            case KEY_DOWN:
            case KEY_RIGHT:
            case 'j':
                highlight++;
                if (highlight == 4) { highlight = 0; }
                break;
            /* TODO: Figure out why 10 != KEY_ENTER */
            case 10:
                init_game(opt);
                should_continue = false;
                delwin(mode_win);
                mode_win = NULL;
                clear();
                break;
            default:
                break;
        } /* switch */
    } /* while */

    highlight = 0;
    board_win = newwin(11, 13, 3, 0);
    wrefresh(board_win);
    keypad(board_win, true);

    while (status == -1) {
        mvprintw(0, 0, "Player %d's turn (turn %d):", curr_player, turn);
        mvprintw(1, 0, "Press ENTER to select a square");
        refresh();

        print_board(board_win, board);

        for (i = 0; i < 9; i++) {
            if (i == highlight) { wattron(board_win, A_STANDOUT); }
            mvwprintw(board_win, 1 + 3 * (i / 3), 3 + 4 * (i % 3), "%c",
                      board[i]);
            wattroff(board_win, A_STANDOUT);
        } /* for */
        opt = wgetch(board_win);

        switch (opt) {
            case KEY_UP:
                highlight -= 3;
                if (highlight < 0) { highlight += 9; }
                break;
            case KEY_DOWN:
                highlight += 3;
                if (highlight > 8) { highlight -= 9; }
                break;
            case KEY_LEFT:
                highlight--;
                if (highlight == -1) { highlight = 8; }
                break;
            case KEY_RIGHT:
                highlight++;
                if (highlight == 9) { highlight = 0; }
                break;
            case 10:
                place_piece(&curr_player, board, highlight, &turn);
                break;
            default:
                break;
        } /* switch */

        /* Check for victory */
        /* Can only win after the 6th turn, hence the check */
        if (turn >= 6) { status = check_for_win(board, turn); }
    } /* while */

    print_board(board_win, board);

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

    mvprintw(16, 0, "Thanks for playing! Press any key to exit.");
    refresh();
    getch();

    delwin(board_win);
    board_win = NULL;

    endwin();
    return 0;
}

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

/* This should do some sort of setting up for the game modes */
void
init_game(int mode)
{
    switch (mode) {
        case MODE_LOCAL_CPU:
        case MODE_LOCAL_PVP:
        case MODE_REMOTE_PVP:
        case MODE_QUIT:
            return;
    } /* switch */
}

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

/* Places a piece and increments the turn counter
 * If the current space is occupied, do nothing */
void
place_piece(int *player, char *board, int pos, int *turn)
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

/* Checks the status of the board
 * Returns -1 if there is no victor yet
 * Returns  0 if there is a tie (reached turn 10, ie board is full)
 * Returns  1 if player 1 (X) won
 * Returns  2 if player 2 (O) won
 */
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
