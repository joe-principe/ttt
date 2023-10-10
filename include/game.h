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
 * @param g The game struct to initialize
 */
void init_game(game *g);

/**
 * Allows the user to select the game mode
 * @param g The game struct to do something
 */
void select_mode(game *g);

/**
 * Sets the gamemode
 * @param g The game whose mode will be set
 * @param mode The chosen gamemode
 */
void set_mode(game *g, int mode);

/**
 * Configures the computer opponent for local CPU games
 * @param g The game to set stuff for
 */
void configure_cpu(game *g);

/**
 * Sets the number of the local player
 * @param g The game to set stuff for
 * @param player_num The number to which the local player is set
 */
void set_local_player(game *g, int player_num);

/**
 * Configures the remote connection for remote PvP games
 * @param g The game to set stuff for
 */
void configure_remote(game *g);

/**
 * The main loop of the game. Called after the player has selected the gamemode
 * @param g The game state struct
 */
void game_loop(game *g);

/**
 * Places a piece on the board
 * @param g The game that contains the board, current player, and turn number
 * @param pos The position on the board to place the piece
 */
void place_piece(game *g, int pos);

/**
 * Checks if the game has been won
 * @param g The game struct to contain the board and turn number
 * @return -1 if nobody has won yet, 0 if there is a tie, 1 if player 1 wins, 2
 * if player 2 wins
 */
int check_for_win(game *g);

#endif
/* EOF */
