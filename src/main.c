#include <stdbool.h>

#include "../include/display.h"
#include "../include/game.h"

int
main(void)
{
    int i;
    int pos_hi = 0, action = 0, status = -1;
    game g;
    WINDOW *board_win;

    /* Initialization stuff */
    init_ncurses();
    init_game(&g);

    select_mode();

    board_win = newwin(11, 13, 3, 0);
    wrefresh(board_win);
    keypad(board_win, true);

    while (status == -1) {
        mvprintw(0, 0, "Player %d's turn (turn %d):", g.curr_player, g.turn);
        mvprintw(1, 0, "Press ENTER to select a square");
        refresh();

        print_board(board_win, g.board);

        for (i = 0; i < 9; i++) {
            if (i == pos_hi) { wattron(board_win, A_STANDOUT); }
            mvwprintw(board_win, 1 + 3 * (i / 3), 3 + 4 * (i % 3), "%c",
                      g.board[i]);
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
                place_piece(g.board, pos_hi, &g.curr_player, &g.turn);
                break;
            default:
                break;
        } /* switch */

        /* Check for victory */
        /* Can only win after the 6th turn, hence the check */
        if (g.turn >= 6) { status = check_for_win(g.board, g.turn); }
    } /* while */

    print_board(board_win, g.board);

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
/* EOF */
