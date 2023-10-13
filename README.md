# multiplayer_tictactoe
Multiplayer, CLI Tic-Tac-Toe game written in C

# Features
- [x] Local Player vs. Player Mode
- [ ] Remote Player vs. Player Mode
- [x] Local Player vs. Computer Mode
- [x] Bot difficulty levels

## Difficult Levels
1. Easy (completely random moves)
2. Medium (random moves but will choose winning move if available)
3. Hard (employs minimax algorithm at various efficiency levels)

# Requirements
* libncurses-dev for ncurses header(s)
	* Requires `#include <ncurses.h>` and `-lncurses` during compilation
