# multiplayer_tictactoe
Multiplayer, CLI Tic-Tac-Toe game written in C

# Features
- [x] Local Player vs. Player Mode
- [ ] Remote Player vs. Player Mode
- [x] Local Player vs. Computer Mode
- [x] Bot difficulty levels

## Difficulty Levels
1. Easy (completely random moves)
2. Medium (makes random moves like easy but will choose a winning move if it's available)
3. Hard Minimax (uses the minimax algorithm to decide on its best move)
4. Hard Trickymax (uses the minimax algorithm but will occasionally make unideal moves to try to trick the opponent)
5. Hard Cache (uses the minimax algorithm with a cache for faster processing)
6. Hard Fastcache (uses the minimax algorithm with a cache that rotates the table for faster caching)
7. Hard Alphabeta (uses fastcache along with alphabeta pruning to go even faster)
8. Hard Precache (uses a premade cache for lookup rather than runtime caching)

Implemented Difficulty Levels:
- [X] Easy
- [X] Medium
- [X] Minimax
- [ ] Trickymax
- [ ] Cache
- [ ] Fastcache
- [ ] Alphabeta
- [ ] Precache

## Note about the bots
If both players are chosen to be bots, there is a one second delay between their
moves so that the game is visible to the player. Otherwise, the game ends as
soon as it starts, which is no fun.

There is no delay if only one of the players is a bot.

# Requirements
* libncurses-dev for ncurses header(s)
	* Requires `#include <ncurses.h>` and `-lncurses` during compilation
