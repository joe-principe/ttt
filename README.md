# ttt
TUI Tic-Tac-Toe game written in C

# Features
- Local Player vs. Player Mode
- Local Player vs. Computer Mode
- Bot difficulty levels
- Command-line interface

## Difficulty Levels
1. Easy (completely random moves)
2. Medium (makes random moves like easy but will choose a winning move if it's available)
3. Hard Minimax (uses the minimax algorithm to decide on its best move)
4. Hard Cache (uses the minimax algorithm with a cache for faster processing)
5. Hard Fastcache (uses the minimax algorithm with a cache that rotates the table for faster caching)
6. Hard Alphabeta (uses alphabeta pruning during minimax to go even faster)

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
