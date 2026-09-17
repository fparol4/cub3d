#include "game.h"

int set_map(t_game *game) {
	static char *grid[] = {
	  "11111111",
	  "10000001",
		"10000001",
	  "10000001",
	  "11111111",
	  NULL
	};
  game->map.grid = grid;
  game->map.width = 8;
  game->map.height = 5;
  return (0);
}

int set_frame(t_game *game) {
  game->frame.width = MLX_WIDTH;
  game->frame.height = MLX_HEIGHT;
  game->frame.ptr = mlx_new_image(
    game->mlx,
    game->frame.width,
    game->frame.height
  );
  if (!game->frame.ptr)
    return (1);
  game->frame.addr = mlx_get_data_addr(
    game->frame.ptr,
    &game->frame.bpp,
    &game->frame.llen,
    &game->frame.endian
  );
  if (!game->frame.addr)
    return (1);
  return (0);
}

int set_mlx(t_game *game)
{
  game->mlx = mlx_init();
  if (!game->mlx)
    return (1);
  game->win = mlx_new_window(game->mlx, MLX_WIDTH, MLX_HEIGHT, G_NAME);
  if (!game->win)
    return (1);
  set_hook(game);
  return (0);
}

int set_hook(t_game *game)
{
	mlx_hook(game->win, MLX_EXIT, 0, game_close, game);
	mlx_key_hook(game->win, hook_key, game);
	return (0);
}
