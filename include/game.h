#ifndef GAME_H
#define GAME_H

typedef struct game_t game;

struct game_t
{
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
 * Sets the gamemode
 * @param mode The chosen gamemode
 */
void set_mode(int mode);

/**
 * Allows the user to select the game mode
 */
void select_mode(void);

/**
 * Places a piece on the board, changes the player, and increments turn counter
 * @param board The board on which the piece is placed
 * @param pos The position on the board to place the piece
 * @param player The player who made the turn
 * @param turn The current turn count
 */
void place_piece(char *board, int pos, int *player, int *turn);

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
