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
void place_piece(int, char *, int);

int
main(void)
{
    int i;
    int highlight = 0, opt = 0;
    int curr_player = 1;
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
    curs_set(1);

    /* Replace true with something */
    while (true) {
        mvprintw(0, 0, "Player %d's turn:", curr_player);
        mvprintw(1, 0, "Press ENTER to select a square");
        refresh();

        mvwprintw(board_win, 0, 0, "     |   |   ");
        mvwprintw(board_win, 1, 0, "A  %c | %c | %c ", board[0], board[1], board[2]);
        mvwprintw(board_win, 2, 0, "  ___|___|___");
        mvwprintw(board_win, 3, 0, "     |   |   ");
        mvwprintw(board_win, 4, 0, "B  %c | %c | %c ", board[3], board[4], board[5]);
        mvwprintw(board_win, 5, 0, "  ___|___|___");
        mvwprintw(board_win, 6, 0, "     |   |   ");
        mvwprintw(board_win, 7, 0, "C  %c | %c | %c ", board[6], board[7], board[8]);
        mvwprintw(board_win, 8, 0, "     |   |   ");
        mvwprintw(board_win, 9, 0, "\n");
        mvwprintw(board_win, 10, 0, "   1 | 2 | 3 ");
        wrefresh(board_win);

        for (i = 0; i < 9; i++) {
            if (i == highlight) { wattron(mode_win, A_REVERSE); }
            mvwprintw(board_win, 1 + 3 * (i / 3), 3 + 4 * (i % 3), "%c",
                      board[i]);
            wattroff(board_win, A_REVERSE);
        } /* for */
        opt = wgetch(board_win);

        switch (opt) {
            case KEY_UP:
                highlight -= 3;
                if (highlight < 0) { highlight += 3; }
                break;
            case KEY_DOWN:
                highlight += 3;
                if (highlight > 8) { highlight -= 3; }
                break;
            case KEY_LEFT:
                highlight--;
                if (highlight == -1) { highlight = 0; }
                break;
            case KEY_RIGHT:
                highlight++;
                if (highlight == 9) { highlight = 8; }
                break;
            case 10:
                place_piece(curr_player, board, highlight);
                curr_player = curr_player % 2 == 1 ? 2 : 1;
                break;
            default:
                break;
        } /* switch */
    } /* while */

    mvprintw(20, 0, "Thanks for playing! Press any key to exit.");
    refresh();
    getch();
    endwin();
    return 0;
}

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
place_piece(int player, char *board, int pos) {
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
/* EOF */
