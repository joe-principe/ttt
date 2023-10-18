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
- [ ] Minimax
- [ ] Trickymax
- [ ] Cache
- [ ] Fastcache
- [ ] Alphabeta
- [ ] Precache

Minimax bot works pretty well if it is player 1. The only scenario I've found
where it doesn't play ideally is "X--O-----". Next move is "XX-O-----", then
"XXOO-----", then "XXOOX----", then any of the following three:
1. "XXOOX-O--"
2. "XXOOX--O-"
3. "XXOOX---O"
All of which result in player 2 losing. The problem is that it always chooses to
move to B3 (row 2, column 3). Other than that, it always plays ideally, at best
winning and at worst tying.

I've been trying to figure out how to make player 2 bot work just as well
because it currently makes terrible moves regardless of who player 1 is. It
seems to prefer to just move to whatever space it finds first, which is even
worse than the easy bot somehow. It will occasionally lose to the easy bot.

# Requirements
* libncurses-dev for ncurses header(s)
	* Requires `#include <ncurses.h>` and `-lncurses` during compilation
