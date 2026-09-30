#include "game.h"
#include <math.h>

static int blocked(t_game *game, double x, double y)
{
	int cell_x;
	int cell_y;

	cell_x = (int)floor(x);
	cell_y = (int)floor(y);
	if (cell_x < 0 || cell_x >= game->map.width
		|| cell_y < 0 || cell_y >= game->map.height)
		return (1);
	return (game->map.grid[cell_y][cell_x] == TILE_WALL);
}

static int can_stand(t_game *game, double x, double y)
{
	double radius;

	radius = 0.18;
	return (!blocked(game, x - radius, y - radius)
		&& !blocked(game, x + radius, y - radius)
		&& !blocked(game, x - radius, y + radius)
		&& !blocked(game, x + radius, y + radius));
}

static void rotate_player(t_player *player, double angle)
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

int update_player(t_game *game, double elapsed)
{
	double forward;
	double strafe;
	double length;
	double move_x;
	double move_y;

	forward = game->keys[GAME_KEY_FORWARD] - game->keys[GAME_KEY_BACKWARD];
	strafe = game->keys[GAME_KEY_STRAFE_RIGHT]
		- game->keys[GAME_KEY_STRAFE_LEFT];
	length = sqrt(forward * forward + strafe * strafe);
	if (length > 0.0)
	{
		forward /= length;
		strafe /= length;
		move_x = (game->player.dir_x * forward
			- game->player.dir_y * strafe) * 2.5 * elapsed;
		move_y = (game->player.dir_y * forward
			+ game->player.dir_x * strafe) * 2.5 * elapsed;
		if (can_stand(game, game->player.pos_x + move_x,
				game->player.pos_y))
			game->player.pos_x += move_x;
		if (can_stand(game, game->player.pos_x,
				game->player.pos_y + move_y))
			game->player.pos_y += move_y;
	}
	if (game->keys[GAME_KEY_TURN_LEFT])
		rotate_player(&game->player, -1.8 * elapsed);
	if (game->keys[GAME_KEY_TURN_RIGHT])
		rotate_player(&game->player, 1.8 * elapsed);
	return (0);
}
