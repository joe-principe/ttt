#ifndef GAME_H
#define GAME_H

typedef struct game_t game;

struct game_t
{
    int local_player;
    int curr_player;
    int turn;
    int mode;
    char board[9];
};

enum gamemodes {
    MODE_LOCAL_CPU = 0,
    MODE_LOCAL_PVP = 1,
    MODE_REMOTE_PVP = 2,
    MODE_QUIT = -1
};

/**
 * Initializes the game struct
 * @param game The game struct to initialize
 */
void init_game(game *game);

/**
 * Allows the user to select the game mode
 */
void select_mode(game *g);

/**
 * Sets the gamemode
 * @param game The game whose mode will be set
 * @param mode The chosen gamemode
 */
void set_mode(game *game, int mode);

/**
 * Configures the computer opponent for local CPU games
 * @param game The game to set stuff for
 */
void configure_cpu(game *game);

/**
 * Sets the number of the local player
 * @param game The game to set stuff for
 * @param player_num The number to which the local player is set
 */
void set_local_player(game *g, int player_num);

/**
 * Configures the remote connection for remote PvP games
 */
void configure_remote(void);

/**
 * The main loop of the game. Called after the player has selected the gamemode
 * @param game The game state struct
 */
void some_function(game *g);

/**
 * Places a piece on the board
 * @param board The board on which the piece is placed
 * @param pos The position on the board to place the piece
 * @param player The player who made the turn
 */
void place_piece(char *board, int pos, int player);

/**
 * Checks if the game has been won
 * @param board The board to check
 * @param turn The current turn count
 * @return -1 if nobody has won yet, 0 if there is a tie, 1 if player 1 wins, 2
 * if player 2 wins
 */
int check_for_win(const char *board, int turn);

#endif
/* EOF */
