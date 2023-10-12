#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct game_t game;
typedef int (*player_move_func_def)(const char *, int);

struct game_t
{
    player_move_func_def player_move_funcptr[2];
    int players[2];
    int cur_player;
    int turn;
    char board[9];
};

enum player_types {
    PLAYER_LOCAL = 0,
    PLAYER_REMOTE = 1,
    PLAYER_COMPUTER = 2
};

enum bot_difficulty {
    BOT_EASY,
    BOT_MEDIUM,
    BOT_MINIMAX,
    BOT_TRICKY,
    BOT_CACHE,
    BOT_FASTCACHE,
    BOT_AB_PRUNING,
    BOT_PRECACHE
};

const char marks[3] = { ' ', 'X', 'O' };

/**
 * Initializes ncurses
 */
void init_ncurses(void);

/**
 * Initializes the game struct
 * @param g The game struct
 */
void init_game(game *g);

/**
 * Sets the player types
 * @param g The game struct containing the players
 */
void set_players(game *g);

/**
 * Sets the player move function pointers
 * @param g The game struct containing the function pointers
 */
void set_player_moves(game *g);

/**
 * Gets a move from a local player
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the player's move
 */
int get_local_move(const char *board, int cur_player);

/**
 * Gets a move from a remote player
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the player's move
 */
int get_remote_move(const char *board, int cur_player);

/**
 * Establishes a connection to a remote player
 */
void establish_connection(void);

/**
 * Sets the difficulty level of a computer opponent
 * @return A pointer to the move function of the corresponding bot difficulty
 */
player_move_func_def set_bot_difficulty(void);

/**
 * Gets a move from an easy bot (places pieces randomly)
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_easy_bot_move(const char *board, int cur_player);

/**
 * Gets a move from a medium bot (places pieces randomly unless winning move is
 * available)
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_medium_bot_move(const char *board, int cur_player);

/**
 * Gets a move from a hard bot (applies the minimax algorithm without any
 * enhancements)
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_minimax_bot_move(const char *board, int cur_player);

/**
 * Gets a move from a hard bot (uses modified minimax to sometimes trick
 * opponent if possible)
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_tricky_bot_move(const char *board, int cur_player);

/**
 * Gets a move from a hard bot (uses minimax with caching)
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_cache_bot_move(const char *board, int cur_player);

/**
 * Gets a move from a hard bot (uses minimax with better caching for when
 * multiple boards are the same state (ie, rotationally equivalent))
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_fastcache_bot_move(const char *board, int cur_player);

/**
 * Gets a move from a hard bot (uses fastcache with alpha beta pruning)
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_ab_pruning_bot_move(const char *board, int cur_player);

/**
 * Gets a move from a hard bot (looks up moves from a cache file)
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_precache_bot_move(const char *board, int cur_player);

/**
 * Performs the main game loop of drawing the board, getting a move, placing a
 * mark, then switching players
 * @param g The game struct
 * @return The final result of the game. 0 for tie, 1/2 for player 1/2 winning
 */
int game_loop(game *g);

/**
 * Prints the current state of the board
 * @param win The window on which to print the board
 * @param board The tic-tac-toe board
 */
void print_board(WINDOW *win, const char *board);

/**
 * Checks the current state of the board for termination.
 * @param board The tic-tac-toe board
 * @param cur_player The player who just moved
 * @return The termination state of the game. -1 if the game should continue, 0
 * if the game is tied, 1/2 if player 1/2 won
 */
int check_for_win(const char *board, int cur_player);

/**
 * Prints the results of the game
 * @param result The final result of the game
 */
void print_results(int result);

int
main(void)
{
    int status;
    game g;

    srand(time(NULL));
    init_ncurses();
    init_game(&g);

    printw("Welcome to Tic-Tac-Toe!");

    set_players(&g);
    set_player_moves(&g);
    status = game_loop(&g);
    print_results(status);

    endwin();
    return 0;
}

/* Initializes ncurses */
void
init_ncurses(void)
{
    initscr();
    noecho();
    cbreak();
    refresh();
    curs_set(0);
}

/* Initializes the game struct */
void
init_game(game *g)
{
    g->cur_player = 1;
    g->turn = 1;
    memset(g->board, ' ', 9);
}

/* Sets the player types */
void
set_players(game *g)
{
    int i, player, key, highlight = 1, num_opts = 3, widest_str_len = 15;
    bool should_continue = true;
    WINDOW *player_select_win;
    const char *player_options[] = {
        "Local Player",
        "Remote Player",
        "Computer Player"
    };

    player_select_win = newwin(num_opts + 2, widest_str_len + 2, 3, 0);
    box(player_select_win, 0, 0);
    wrefresh(player_select_win);
    keypad(player_select_win, true);

    for (player = 1; player < 3; player++) {
        mvprintw(1, 0, "Choose player %d (%c):", player, marks[player]);
        refresh();

        /* Sets the player type for player 1 */
        while (should_continue) {
            for (i = 1; i <= num_opts; i++) {
                if (i == highlight) { wattron(player_select_win, A_STANDOUT); }
                mvwprintw(player_select_win, i, 1, "%s", player_options[i - 1]);
                wattroff(player_select_win, A_STANDOUT);
            } /* for */
            wrefresh(player_select_win);
            key = wgetch(player_select_win);

            switch (key) {
                case KEY_UP:
                case 'w':
                    highlight--;
                    if (highlight < 1) { highlight = num_opts; }
                    break;
                case KEY_DOWN:
                case 's':
                    highlight++;
                    if (highlight > num_opts) { highlight = 1; }
                    break;
                case 10:
                    g->players[player - 1] = highlight;
                    should_continue = false;
                    break;
                default:
                    break;
            } /* switch */
        } /* while */
    } /* for */

    delwin(player_select_win);
    player_select_win = NULL;
    clear();
}

/* Sets the player move function pointers */
void 
set_player_moves(game *g)
{
    int i;

    for (i = 0; i < 2; i++) {
        switch (g->players[i]) {
            case PLAYER_LOCAL:
                g->player_move_funcptr[i] = get_local_move;
                break;
            case PLAYER_REMOTE:
                g->player_move_funcptr[i] = get_remote_move;
                establish_connection();
                break;
            case PLAYER_COMPUTER:
                g->player_move_funcptr[i] = set_bot_difficulty();
                break;
            default:
                break;
        } /* switch */
    } /* for */
}

/* Gets a move from a local player */
int
get_local_move(const char *board, int cur_player)
{
    int i, pos, key, pos_hi = 0;
    bool should_continue = true;

    /* Unfortunately, since I can't pass in the board window, I just highlight
     * on stdscr. This works just fine, but it doesn't have the nice separation
     * I get from using board_win
     * Also, I'm not actually sure if this'll work, so hope for the best */
    while (should_continue) {
        for (i = 0; i < 9; i++) {
            if (i == pos_hi) { attron(A_STANDOUT); }
            mvprintw(3 + 3 * (i / 3), 3 + 4 * (i % 3), "%c", board[i]);
            attroff(A_STANDOUT);
        } /* for */
        key = getch();

        switch (key) {
            case KEY_UP:
            case 'w':
                pos_hi -= 3;
                if (pos_hi < 0) { pos_hi += 9; }
                break;
            case KEY_DOWN:
            case 's':
                pos_hi += 3;
                if (pos_hi > 8) { pos_hi -= 9; }
                break;
            case KEY_LEFT:
            case 'a':
                pos_hi--;
                if (pos_hi == -1) { pos_hi = 8; }
                break;
            case KEY_RIGHT:
            case 'd':
                pos_hi++;
                if (pos_hi == 9) { pos_hi = 0; }
                break;
            case 10:
                if (board[pos_hi] == ' ') {
                    pos = pos_hi;
                    should_continue = false;
                }
                break;
            default:
                break;
        } /* switch */
    } /* while */

    return pos;
}

/* Gets a move from a remote player */
int
get_remote_move(const char *board, int cur_player);

/* Establishes a connection to a remote player */
void 
establish_connection(void);

/* Sets the difficulty level of a computer opponent */
player_move_func_def
set_bot_difficulty(void)
{
    int i, key, diff_hi = 0, num_opts = 8, widest_str_len = 36;
    bool should_continue = true;
    player_move_func_def diff_mode;
    WINDOW *diff_win;
    const char *difficulty_options[] = {
        "Easy",
        "Medium",
        "Hard (Minimax)",
        "Hard (Tricky Minimax)",
        "Hard (Minimax w/ cache)",
        "Hard (Minimax w/ fast cache)",
        "Hard (Minimax w/ alpha beta pruning)",
        "Hard (Precache)"
    };
    player_move_func_def bot_move_funcs[8] = {
        get_easy_bot_move,
        get_medium_bot_move,
        get_minimax_bot_move,
        get_tricky_bot_move,
        get_cache_bot_move,
        get_fastcache_bot_move,
        get_ab_pruning_bot_move,
        get_precache_bot_move
    };

    diff_win = newwin(num_opts + 2, widest_str_len + 2, 2, 0);
    box(diff_win, 0, 0);
    wrefresh(diff_win);
    keypad(diff_win, true);

    mvprintw(0, 0, "Choose bot difficulty:" );

    while (should_continue) {
        for (i = 0; i < num_opts; i++) {
            if (i == diff_hi) { wattron(diff_win, A_STANDOUT); }
            mvwprintw(diff_win, i + 1, 1, "%s", difficulty_options[i]);
            wattroff(diff_win, A_STANDOUT);
        } /* for */
        key = wgetch(diff_win);

        switch (key) {
            case KEY_UP:
            case 'w':
                diff_hi--;
                if (diff_hi < 0) { diff_hi = num_opts - 1; }
                break;
            case KEY_DOWN:
            case 's':
                diff_hi++;
                if (diff_hi >= num_opts) { diff_hi = 0; }
                break;
            case 10:
                diff_mode = bot_move_funcs[diff_hi];
                break;
            default:
                break;
        } /* switch */
    } /* while */

    return diff_mode;
}

/* Gets a move from an easy bot (places pieces randomly) */
int 
get_easy_bot_move(const char *board, int cur_player)
{
    int i, j, pos, num_empty = 0;
    int *empty_pos;

    for (i = 0; i < 9; i++) {
        if (board[i] == ' ') { num_empty++; }
    } /* for */

    empty_pos = malloc(sizeof(int) * num_empty);

    if (empty_pos == NULL) {
        fprintf(stderr, "Error: Could not allocate enough memory in file %s "
                "@ line: %d", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    } /* if */

    j = 0;
    for (i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            empty_pos[j] = i;
            j++;
        } /* if */
    } /* for */

    pos = empty_pos[rand() % num_empty];

    free(empty_pos);
    empty_pos = NULL;

    return pos;
}

/* Gets a move from a medium bot (places pieces randomly unless winning move is
 * available) */
int 
get_medium_bot_move(const char *board, int cur_player)
{
    int i, j, pos, sum = 0;
    char mark = marks[cur_player];

    for (i = 0; i < 9; i += 3) {
        for (j = i; j < 9 - (6 - i); j++) {
            if (board[j] == ' ') { pos = j; }
            if (board[j] == mark) { sum++; }
        } /* for */
        if (sum == 2) { return pos; }
        sum = 0;
    } /* for */

    for (i = 0; i < 3; i++) {
        for (j = i; j < i + 7; j += 3) {
            if (board[j] == ' ') { pos = j; }
            if (board[j] == mark)  { sum++; }
        } /* for */
        if (sum == 2) { return pos; }
        sum = 0;
    } /* for */

    for (i = 0; i < 9; i += 4) {
        if (board[i] == ' ') { pos = i; }
        if (board[i] == mark) { sum++; }
    } /* for */
    if (sum == 2) { return pos; }
    sum = 0;

    for (i = 2; i < 7; i += 2) {
        if (board[i] == ' ') { pos = i; }
        if (board[i] == mark) { sum++; }
    } /* for */
    if (sum == 2) { return pos; }

    return get_easy_bot_move(board, cur_player);
}

/* Gets a move from a hard bot (applies the minimax algorithm without any
 * enhancements) */
int 
get_minimax_bot_move(const char *board, int cur_player);

/* Gets a move from a hard bot (uses modified minimax to sometimes trick
 * opponent if possible) */
int 
get_tricky_bot_move(const char *board, int cur_player);

/* Gets a move from a hard bot (uses minimax with caching)
 * @param board The tic-tac-toe board */
int 
get_cache_bot_move(const char *board, int cur_player);

/* Gets a move from a hard bot (uses minimax with better caching for when
 * multiple boards are the same state (ie, rotationally equivalent)) */
int 
get_fastcache_bot_move(const char *board, int cur_player);

/* Gets a move from a hard bot (uses fastcache with alpha beta pruning) */
int 
get_ab_pruning_bot_move(const char *board, int cur_player);

/* Gets a move from a hard bot (looks up moves from a cache file) */
int 
get_precache_bot_move(const char *board, int cur_player);

/* Performs the main game loop of drawing the board, getting a move, placing a
 * mark, then switching players */
int 
game_loop(game *g)
{
    int pos, status = -1;
    WINDOW *board_win;

    board_win = newwin(11, 13, 3, 0);
    wrefresh(board_win);
    keypad(board_win, true);

    while (status == -1) {
        mvprintw(0, 0, "Player %d's (%c) turn (turn %d):", g->cur_player,
                 marks[g->cur_player], g->turn);
        refresh();

        print_board(board_win, g->board);

        pos = (*g->player_move_funcptr[g->cur_player - 1])(g->board,
                                                           g->cur_player);
        g->board[pos] = marks[g->cur_player];
        g->cur_player = g->cur_player == 1 ? 2 : 1;
        g->turn++;

        /* Check for victory */
        /* Can only win after the 6th turn, hence the check */
        if (g->turn >= 6) { status = check_for_win(g->board, g->turn); }
    } /* while */

    print_board(board_win, g->board);

    return status;
}

/* Prints the current state of the board */
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
    mvwprintw(win, 10, 0, "   1 | 2 | 3 ");
    wrefresh(win);
}

/* Checks the current state of the board for termination. */
int 
check_for_win(const char *board, int turn)
{
    int i;

    for (i = 0; i < 9; i += 3) {
        if (board[i] == board[i + 1]
         && board[i + 1] == board[i + 2]) {
            return board[i] == 'X' ? 1 : 2;
        } /* if */
    } /* for */

    /* Check the columns for a victor */
    for (i = 0; i < 3; i++) {
        if (board[i] == board[i + 3]
         && board[i + 3] == board[i + 6]) {
            return board[i] == 'X' ? 1 : 2;
        } /* if */
    } /* for */

    /* Checks the backslash diagonal */
    if (board[0] == board[4] && board[4] == board[8]) {
        return board[4] == 'X' ? 1 : 2;
    } /* if */

    /* Checks the forwardslash diagonal */
    if (board[6] == board[4] && board[4] == board[2]) {
        return board[4] == 'X' ? 1 : 2;
    } /* if */

    /* Checks if the board is full with no winner */
    if (turn == 10) { return 0; }

    /* Continues the game */
    return -1;
}

/* Prints the results of the game */
void
print_results(int result)
{
    switch (result) {
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
}
/* EOF */
