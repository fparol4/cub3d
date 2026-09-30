#include "../cub3d.h"
#include "game/game.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  t_game g;

  if (game_setup(&g))
  	return (1);

  mlx_loop(g.mlx);

  game_cleanup(&g);
  return (0);
}
