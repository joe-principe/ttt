# multiplayer_tictactoe
Multiplayer, CLI Tic-Tac-Toe game written in C

# Features
- [x] Local Player vs. Player Mode
- [ ] Remote Player vs. Player Mode
- [x] Local Player vs. Computer Mode
- [x] Bot difficulty levels
- [x] Command-line interface
- [ ] Graphical user interface

## Difficulty Levels
1. Easy (completely random moves)
2. Medium (makes random moves like easy but will choose a winning move if it's available)
3. Hard Minimax (uses the minimax algorithm to decide on its best move)
4. Hard Cache (uses the minimax algorithm with a cache for faster processing)
5. Hard Fastcache (uses the minimax algorithm with a cache that rotates the table for faster caching)
6. Hard Alphabeta (uses alphabeta pruning during minimax to go even faster)
7. Hard Precache (uses a premade cache for lookup rather than runtime caching)

Implemented Difficulty Levels:
- [X] Easy
- [X] Medium
- [X] Minimax
- [X] Cache
- [X] Fastcache
- [X] Alphabeta
- [ ] Precache

## Note about the bots
If both players are chosen to be bots, there is a one second delay between their
moves so that the game is visible to the player. Otherwise, the game ends as
soon as it starts, which is no fun.

There is no delay if only one of the players is a bot.

There is currently an issue if both bots are the same type of cache bot (both
cache, both fastcache, both alphabeta). I think it has something to do with
overwriting the results in the hashtable. Otherwise, they work just fine.

The alpha-beta bot kinda sorta works. Idk what's wrong with it right now and I
don't really wanna keep debugging it to find out.

# Requirements
* libncurses-dev for ncurses header(s)
	* Requires `#include <ncurses.h>` and `-lncurses` during compilation
