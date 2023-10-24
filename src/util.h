#ifndef UTIL_H
#define UTIL_H

typedef struct game_t game;
typedef int (*player_move_func)(const char *, int);

struct game_t
{
    player_move_func player_move_funcptr[2];
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
    BOT_CACHE,
    BOT_FASTCACHE,
    BOT_AB_PRUNING,
    BOT_PRECACHE
};

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
 * @param players An array containing the chosen player types
 */
void set_players(int *players);

/**
 * Sets the player move function pointers
 * @param players An array containing the chosen player types
 * @param player_move_funcptr An array containing the player move functions
 */
void set_player_moves(int *players, player_move_func *player_move_funcptr);

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
player_move_func set_bot_difficulty(int player);

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
 * Gets the score of a potential move on the board
 * @param board The tic-tac-toe board
 * @param player_to_move The player whose turn it is
 * @param player_to_optimize The player whose score we want to maximize
 * @param depth The number of turns that have already been made
 * @return For a terminal state, returns 0 for tie, +10 if the player to move is
 * the player to optimize, -10 otherwise. If the board is not terminal, return
 * the best score from the states below if the player to move is the player to
 * optimize, otherwise return the worst score.
 */
int minimax_score(const char *board, int player_to_move, int player_to_optimize,
                  int depth);

/**
 * Gets all of the legal moves on the board.
 * @note The legal moves array contains the positions of legal moves. eg, only
 * one legal move would be [pos, garbage, ..., -1]
 * @param board The tic-tac-toe board
 * @param legal_moves An array of the positions of legal moves. Passed in as an
 * out value. Illegal moves are -1, which the board is initialized to
 * @return The number of legal moves for the board
 */
int get_legal_moves(const char *board, int *legal_moves);

/**
 * Gets a move from a hard bot (uses minimax with caching)
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_cache_bot_move(const char *board, int cur_player);

/**
 * Gets the score of a potential move on the board, either from a hashtable if
 * the board has been cached or recursive searching if it has not yet been
 * cached
 * @param board The tic-tac-toe board
 * @param player_to_move The player whose turn it is
 * @param player_to_optimize The player whose score we want to maximize
 * @param depth The number of turns that have already been made
 * @return The result of the board. 0 if tie, -10 if the opponent won, +10 if
 * the player to optimize won
 */
int minimax_cache_score(const char *board, int player_to_move,
                        int player_to_optimize, int depth);

/**
 * Converts a game score into an end game result
 * @param score The score to convert
 * @param player The current player
 * @param opponent The opponent player
 * @return The result of the game. 0 if tie, 1/2 if player 1/2 wins
 */
int score_to_result(int score, int player, int opponent);

/**
 * Converts an end game result into a game score
 * @param score The score to convert
 * @param player The current player
 * @param opponent The opponent player
 * @return The score of the game. -10 if the opponent won, 0 if tie, +10 if the
 * player to optimize won
 */
int result_to_score(int result, int player, int opponent);

/**
 * Gets a move from a hard bot (uses minimax with better caching for when
 * multiple boards are the same state (ie, rotationally equivalent))
 * @param board The tic-tac-toe board
 * @param cur_player The player whose turn it is
 * @return The position of the bot's move
 */
int get_fastcache_bot_move(const char *board, int cur_player);

/**
 * Gets the score of a potential move on the board, either from a hash table if
 * the board has been cached or recursive searching if it has not yet been
 * hashed. Also caches rotations of the board since a board and all of its
 * rotations share the same score
 * @param board The tic-tac-toe board
 * @param player_to_move The player whose turn it is
 * @param player_to_optimize The player whose score we want to maximize
 * @param depth The number of turns that have already been made
 * @return The result of the board. 0 if tie, -10 if the opponent won, +10 if
 * the player to optimize won
 */
int minimax_fastcache_score(const char *board, int player_to_move,
                            int player_to_optimize, int depth);

/**
 * Rotates the board clockwise to get new orientations for caching.
 * @param original_board The original, unaltered board
 * @param r90deg_board The original board rotated 90 degrees clockwise
 * @param r180deg_board The original board rotated 180 degrees clockwise
 * @param r270deg_board The original board rotated 270 degrees clockwise
 */
void rotate_board(const char *original_board, char *r90deg_board,
                  char *r180deg_board, char *r270deg_board);

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

#endif
/* EOF */
