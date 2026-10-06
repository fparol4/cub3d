#include "../../cub3d.h"
#include <math.h>

int input_key_index(int keycode)
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

int player_cell_blocked(t_game *game, double x, double y)
{
	int cell_x;
	int cell_y;

	cell_x = (int)floor(x);
	cell_y = (int)floor(y);
	if (cell_x < 0 || cell_x >= game->map.width
		|| cell_y < 0 || cell_y >= game->map.height)
		return (1);
	return (game->map.grid[cell_y][cell_x] != TILE_FLOOR);
}

int player_can_stand(t_game *game, double x, double y)
{
	double radius;

	radius = 0.18;
	return (!player_cell_blocked(game, x - radius, y - radius)
		&& !player_cell_blocked(game, x + radius, y - radius)
		&& !player_cell_blocked(game, x - radius, y + radius)
		&& !player_cell_blocked(game, x + radius, y + radius));
}

void player_rotate(t_player *player, double angle)
{
	double old_x;
	double cosine;
	double sine;

	cosine = cos(angle);
	sine = sin(angle);
	old_x = player->dir_x;
	player->dir_x = player->dir_x * cosine - player->dir_y * sine;
	player->dir_y = old_x * sine + player->dir_y * cosine;
	old_x = player->plane_x;
	player->plane_x = player->plane_x * cosine - player->plane_y * sine;
	player->plane_y = old_x * sine + player->plane_y * cosine;
}
