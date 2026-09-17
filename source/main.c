#include "../cub3d.h"
#include "game/game.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  t_game g;

  if (game_setup(&g))
  	return (1);

  g.player.pos_x = 5;
  g.player.pos_y = 5;

  render_map(&g);
  render_player(&g);

  mlx_put_image_to_window(g.mlx, g.win, g.frame.ptr, 0, 0);
  mlx_loop(g.mlx);

  game_cleanup(&g);
  return (0);
}
