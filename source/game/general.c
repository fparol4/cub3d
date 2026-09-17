#include "game.h"

int game_close(t_game *game) {
  mlx_loop_end(game->mlx);
  return (0);
}

int game_cleanup(t_game *game) {
  if (game->frame.ptr)
    mlx_destroy_image(game->mlx, game->frame.ptr);
  if (game->win)
    mlx_destroy_window(game->mlx, game->win);
  if (game->mlx) {
    mlx_destroy_display(game->mlx);
    free(game->mlx);
  }
  return (0);
}

int game_setup(t_game *game) {

  *game = (t_game){0};
  if (set_map(game) || set_mlx(game) || set_frame(game))
  {
    game_cleanup(game);
    return (1);
  }
  return (0);
}
