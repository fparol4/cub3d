#include "../cub3d.h"

int hook_key_press(int keycode, t_game *game)
{
	int index;

	if (keycode == K_ESC)
		return (game_close(game));
	if (keycode == K_M)
	{
		/* Ignore X11's synthetic release/press pair for key repeat. */
		if (game->view_key_release_time > 0.0
			&& game_time() - game->view_key_release_time < 0.02)
		{
			game->view_key_release_time = 0.0;
			game->keys[GAME_KEY_VIEW] = 1;
			return (0);
		}
		game->view_key_release_time = 0.0;
		if (!game->keys[GAME_KEY_VIEW])
			game->view_mode = (game->view_mode == VIEW_MINIMAP)
				? VIEW_3D : VIEW_MINIMAP;
		game->keys[GAME_KEY_VIEW] = 1;
		return (0);
	}
	index = input_key_index(keycode);
	if (index >= 0 && index < GAME_KEY_COUNT)
		game->keys[index] = 1;
	return (0);
}

int hook_key_release(int keycode, t_game *game)
{
	int index;

	if (keycode == K_M)
		game->view_key_release_time = game_time();
	index = input_key_index(keycode);
	if (keycode == K_M)
		index = GAME_KEY_VIEW;
	if (index >= 0 && index < GAME_KEY_COUNT)
		game->keys[index] = 0;
	return (0);
}

int hook_focus_out(t_game *game)
{
	int i;

	i = 0;
	while (i < GAME_KEY_COUNT)
		game->keys[i++] = 0;
	game->view_key_release_time = 0.0;
	return (0);
}
