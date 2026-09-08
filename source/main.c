#include "../header/cub3d.h"
#include <stdlib.h>
#include <stdio.h>

int game_close(t_game *game) {
  if (game->win)
    mlx_destroy_window(game->mlx, game->win);
  if (game->mlx) {
    mlx_destroy_display(game->mlx);
    free(game->mlx);
  }
  exit(0);
  return (0);
}

int hook_key(int keycode, t_game *game) {
  if (keycode == K_ESC)
    game_close(game);
  return (0);
}

int main(int argc, char **argv) {
	t_game game;
	(void)argc;
	(void)argv;

	game.mlx = mlx_init();
	if (!game.mlx)
		return (1);

	game.win = mlx_new_window(game.mlx,  MLX_WIDTH, MLX_HEIGHT, G_NAME);
	if (!game.win)
		return (game_close(&game), 1);

	mlx_hook(game.win, MLX_EXIT, 0, game_close, &game);
	mlx_key_hook(game.win, hook_key,  &game);
	mlx_loop(game.mlx);

  return 0;
}
