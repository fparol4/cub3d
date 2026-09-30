#include "game.h"

static int key_index(int keycode)
{
	if (keycode == K_W || keycode == K_UP)
		return (GAME_KEY_FORWARD);
	if (keycode == K_S || keycode == K_DOWN)
		return (GAME_KEY_BACKWARD);
	if (keycode == K_A)
		return (GAME_KEY_STRAFE_LEFT);
	if (keycode == K_D)
		return (GAME_KEY_STRAFE_RIGHT);
	if (keycode == K_LEFT)
		return (GAME_KEY_TURN_LEFT);
	if (keycode == K_RIGHT)
		return (GAME_KEY_TURN_RIGHT);
	return (-1);
}

int hook_key_press(int keycode, t_game *game)
{
	int index;

	if (keycode == K_ESC)
		return (game_close(game));
	index = key_index(keycode);
	if (index >= 0 && index < GAME_KEY_COUNT)
		game->keys[index] = 1;
	return (0);
}

int hook_key_release(int keycode, t_game *game)
{
	int index;

	index = key_index(keycode);
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
	return (0);
}
