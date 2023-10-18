#include "util.h"

int
main(void)
{
    int status;
    game g;

    init_ncurses();
    init_game(&g);
    set_players(g.players);
    set_player_moves(g.players, g.player_move_funcptr);
    status = game_loop(&g);
    print_results(status);

    return 0;
}
/* EOF */
