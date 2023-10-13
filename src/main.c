#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

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
 * @param player The player number of the bot (either 1 or 2)
 * @return A pointer to the move function of the corresponding bot difficulty
 */
player_move_func_def set_bot_difficulty(int player);

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

/* TODO: Enter the description of this function */
int minimax_score(const char *board, int opponent, int cur_player);

/**
 * Gets all of the legal moves on the board.
 * @note The legal moves array contains the positions of legal moves followed by
 * -1 until the end. eg, only one legal move would be [pos, -1, ..., -1]
 * @param board The tic-tac-toe board
 * @param legal_moves An array of the positions of legal moves. Passed in as an
 * out value. Illegal moves are -1, which the board is initialized to
 * @return The number of legal moves for the board
 */
int get_legal_moves(const char *board, int *legal_moves);

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
void print_board(const char *board);

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
    keypad(stdscr, true);
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
        should_continue = true;
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
                    g->players[player - 1] = highlight - 1;
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
    refresh();
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
                g->player_move_funcptr[i] = set_bot_difficulty(i + 1);
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

    while (should_continue) {
        for (i = 0; i < 9; i++) {
            if (i == pos_hi) { attron(A_STANDOUT); }
            mvprintw(3 + 3 * (i / 3), 3 + 4 * (i % 3), "%c", board[i]);
            attroff(A_STANDOUT);
        } /* for */
        refresh();
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
get_remote_move(const char *board, int cur_player)
{
    return 0;
}

/* Establishes a connection to a remote player */
void 
establish_connection(void)
{
    return;
}

/* Sets the difficulty level of a computer opponent */
player_move_func_def
set_bot_difficulty(int player)
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

    mvprintw(0, 0, "Choose player %d bot difficulty:", player );
    refresh();

    while (should_continue) {
        for (i = 0; i < num_opts; i++) {
            if (i == diff_hi) { wattron(diff_win, A_STANDOUT); }
            mvwprintw(diff_win, i + 1, 1, "%s", difficulty_options[i]);
            wattroff(diff_win, A_STANDOUT);
        } /* for */
        wrefresh(diff_win);
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
                should_continue = false;
                break;
            default:
                break;
        } /* switch */
    } /* while */

    delwin(diff_win);
    diff_win = NULL;
    clear();
    refresh();

    return diff_mode;
}

/* Gets a move from an easy bot (places pieces randomly) */
int 
get_easy_bot_move(const char *board, int cur_player)
{
    int pos, num_empty = 0;
    int empty_pos[9];

    num_empty = get_legal_moves(board, empty_pos);

    pos = empty_pos[rand() % num_empty];

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
get_minimax_bot_move(const char *board, int cur_player)
{
    int i, best_pos, score, best_score = 0, num_empty, opponent;
    int legal_moves[9];
    char mark = cur_player == 1 ? 'X' : 'O';
    char new_board[9];

    for (i = 0; i < 9; i++) { new_board[i] = board[i]; }

    num_empty = get_legal_moves(board, legal_moves);

    for (i = 0; i < num_empty; i++) {
        new_board[legal_moves[i]] = mark;
        opponent = cur_player == 1 ? 2 : 1;
        score = minimax_score(new_board, opponent, cur_player);
        if (score > best_score) {
            best_pos = legal_moves[i];
            best_score = score;
        } /* if */
    } /* for */
    return best_pos;
}

/* Gets the minimax score */
int 
minimax_score(const char *board, int opponent, int cur_player)
{
    int i, num_empty, status, max_score = 0, min_score = 0, score = 0;
    int legal_moves[9], scores[9];
    char mark = cur_player == 1 ? 'X' : 'O';
    char new_board[9];
    
    for (i = 0; i < 9; i++) { new_board[i] = board[i]; }

    status = check_for_win(board, cur_player);

    if (status != -1) {
        if (status == 0) { return 0; }
        else if (status == cur_player) { return 10; }
        else { return -10; }
    } /* else if */

    num_empty = get_legal_moves(board, legal_moves);

    for (i = 0; i < num_empty; i++) {
        new_board[legal_moves[i]] = mark;
        opponent = cur_player == 1 ? 2 : 1;
        score = minimax_score(new_board, opponent, cur_player);
        if (score > max_score) { max_score = score; }
        if (score < min_score) { min_score = score; }
        scores[i] = score;
    } /* for */

    if (cur_player == opponent) { return max_score; }
    return min_score;
}

/* Gets the current legal moves on the board */
int
get_legal_moves(const char *board, int *legal_moves)
{
    int i, j = 0;

    for (i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            legal_moves[j] = i;
            j++;
        } /* if */
    } /* for */

    return j;
}

/* Gets a move from a hard bot (uses modified minimax to sometimes trick
 * opponent if possible) */
int 
get_tricky_bot_move(const char *board, int cur_player)
{
    return 0;
}

/* Gets a move from a hard bot (uses minimax with caching)
 * @param board The tic-tac-toe board */
int 
get_cache_bot_move(const char *board, int cur_player)
{
    return 0;
}

/* Gets a move from a hard bot (uses minimax with better caching for when
 * multiple boards are the same state (ie, rotationally equivalent)) */
int 
get_fastcache_bot_move(const char *board, int cur_player)
{
    return 0;
}

/* Gets a move from a hard bot (uses fastcache with alpha beta pruning) */
int 
get_ab_pruning_bot_move(const char *board, int cur_player)
{
    return 0;
}

/* Gets a move from a hard bot (looks up moves from a cache file) */
int 
get_precache_bot_move(const char *board, int cur_player)
{
    return 0;
}

/* Performs the main game loop of drawing the board, getting a move, placing a
 * mark, then switching players */
int 
game_loop(game *g)
{
    int pos, status = -1;

    while (status == -1) {
        mvprintw(0, 0, "Player %d's turn (%c) (turn %d):", g->cur_player,
                 marks[g->cur_player], g->turn);
        refresh();
        print_board(g->board);

        pos = (*g->player_move_funcptr[g->cur_player - 1])(g->board,
                                                           g->cur_player);
        g->board[pos] = marks[g->cur_player];
        if (g->players[g->cur_player - 1] == PLAYER_COMPUTER) {
#ifdef _WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
        }
        g->cur_player = g->cur_player == 1 ? 2 : 1;
        g->turn++;

        /* Check for victory */
        /* Can only win after the 5th turn, hence the check */
        if (g->turn > 5) { status = check_for_win(g->board, g->turn); }
    } /* while */

    print_board(g->board);

    return status;
}

/* Prints the current state of the board */
void 
print_board(const char *board)
{
    mvprintw( 2, 0, "     |   |   ");
    mvprintw( 3, 0, "A  %c | %c | %c ", board[0], board[1], board[2]);
    mvprintw( 4, 0, "  ___|___|___");
    mvprintw( 5, 0, "     |   |   ");
    mvprintw( 6, 0, "B  %c | %c | %c ", board[3], board[4], board[5]);
    mvprintw( 7, 0, "  ___|___|___");
    mvprintw( 8, 0, "     |   |   ");
    mvprintw( 9, 0, "C  %c | %c | %c ", board[6], board[7], board[8]);
    mvprintw(10, 0, "     |   |   ");
    mvprintw(12, 0, "   1 | 2 | 3 ");
    refresh();
}

/* Checks the current state of the board for termination. */
int 
check_for_win(const char *board, int turn)
{
    int i;

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
        if (board[4] == 'X') { return 1; }
        else if (board[4] == 'O') { return 2; }
    } /* if */

    /* Checks the forwardslash diagonal */
    if (board[6] == board[4] && board[4] == board[2]) {
        if (board[4] == 'X') { return 1; }
        else if (board[4] == 'O') { return 2; }
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
