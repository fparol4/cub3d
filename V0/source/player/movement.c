#include "../../cub3d.h"
#include <math.h>

void player_move(t_game *game, double forward, double strafe, double elapsed)
{
	double length;
	double move_x;
	double move_y;

	length = sqrt(forward * forward + strafe * strafe);
	if (length == 0.0)
		return ;
	forward /= length;
	strafe /= length;
	move_x = (game->player.dir_x * forward - game->player.dir_y * strafe)
		* 2.5 * elapsed;
	move_y = (game->player.dir_y * forward + game->player.dir_x * strafe)
		* 2.5 * elapsed;
	if (player_can_stand(game, game->player.pos_x + move_x, game->player.pos_y))
		game->player.pos_x += move_x;
	if (player_can_stand(game, game->player.pos_x, game->player.pos_y + move_y))
		game->player.pos_y += move_y;
}

int update_player(t_game *game, double elapsed)
{
	double forward;
	double strafe;

	forward = game->keys[GAME_KEY_FORWARD] - game->keys[GAME_KEY_BACKWARD];
	strafe = game->keys[GAME_KEY_STRAFE_RIGHT]
		- game->keys[GAME_KEY_STRAFE_LEFT];
	player_move(game, forward, strafe, elapsed);
	if (game->keys[GAME_KEY_TURN_LEFT])
		player_rotate(&game->player, -1.8 * elapsed);
	if (game->keys[GAME_KEY_TURN_RIGHT])
		player_rotate(&game->player, 1.8 * elapsed);
	return (0);
}
