#include "../../cub3d.h"
#include <string.h>

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
	memset(game->frame.addr, 0, game->frame.llen * game->frame.height);
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
	mlx_hook(game->win, MLX_EVENT_KEY_PRESS, MLX_MASK_KEY_PRESS,
		hook_key_press, game);
	mlx_hook(game->win, MLX_EVENT_KEY_RELEASE, MLX_MASK_KEY_RELEASE,
		hook_key_release, game);
	mlx_hook(game->win, MLX_EVENT_FOCUS_OUT, MLX_MASK_FOCUS_CHANGE,
		hook_focus_out, game);
	mlx_loop_hook(game->mlx, game_frame, game);
	return (0);
}
