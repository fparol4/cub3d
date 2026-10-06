#include "../../cub3d.h"
#include "libft.h"
#include <string.h>

int game_frame(t_game *game)
{
	double now;
	double elapsed;

	now = game_time();
	elapsed = now - game->last_frame;
	game->last_frame = now;
	if (elapsed < 0.0)
		elapsed = 0.0;
	if (elapsed > 0.05)
		elapsed = 0.05;
	update_player(game, elapsed);
	memset(game->frame.addr, 0, game->frame.llen * game->frame.height);
	if (game->view_mode == VIEW_3D)
		render_3d(game);
	else
	{
		render_map(game);
		render_fov(game);
		render_player(game);
	}
	mlx_put_image_to_window(game->mlx, game->win, game->frame.ptr, 0, 0);
	return (0);
}

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
	map_destroy(&game->map);
  return (0);
}

int game_setup(t_game *game, const char *map_path) {

  *game = (t_game){0};
  if (parse_cub_file(game, map_path))
  {
    game_cleanup(game);
    return (1);
  }
  if (set_mlx(game))
  {
    ft_putendl_fd("cub3d: could not initialize MiniLibX (is a graphical display available?)", 2);
    game_cleanup(game);
    return (1);
  }
  if (set_frame(game))
  {
    ft_putendl_fd("cub3d: could not create the render frame", 2);
    game_cleanup(game);
    return (1);
  }
  return (0);
}
