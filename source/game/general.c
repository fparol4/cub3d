#include "game.h"
#include <string.h>
#include <sys/time.h>

static double get_time(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec + time.tv_usec / 1000000.0);
}

int game_frame(t_game *game)
{
	double now;
	double elapsed;

	now = get_time();
	elapsed = now - game->last_frame;
	game->last_frame = now;
	if (elapsed < 0.0)
		elapsed = 0.0;
	if (elapsed > 0.05)
		elapsed = 0.05;
	update_player(game, elapsed);
	memset(game->frame.addr, 0, game->frame.llen * game->frame.height);
	render_map(game);
	render_fov(game);
	render_player(game);
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
