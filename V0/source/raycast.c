#include "../cub3d.h"
#include <math.h>

void ray_setup(t_game *game, double ray_x, double ray_y, t_ray_state *state)
{
	state->map_x = (int)floor(game->player.pos_x);
	state->map_y = (int)floor(game->player.pos_y);
	state->delta_x = (ray_x == 0.0) ? 1e30 : fabs(1.0 / ray_x);
	state->delta_y = (ray_y == 0.0) ? 1e30 : fabs(1.0 / ray_y);
	state->step_x = (ray_x < 0.0) ? -1 : 1;
	state->step_y = (ray_y < 0.0) ? -1 : 1;
	state->side_x = ray_x < 0.0
		? (game->player.pos_x - state->map_x) * state->delta_x
		: (state->map_x + 1.0 - game->player.pos_x) * state->delta_x;
	state->side_y = ray_y < 0.0
		? (game->player.pos_y - state->map_y) * state->delta_y
		: (state->map_y + 1.0 - game->player.pos_y) * state->delta_y;
}

void ray_advance(t_ray_state *state, t_ray_hit *hit)
{
	if (state->side_x < state->side_y)
	{
		hit->distance = state->side_x;
		state->side_x += state->delta_x;
		state->map_x += state->step_x;
		state->side = RAY_SIDE_X;
	}
	else
	{
		hit->distance = state->side_y;
		state->side_y += state->delta_y;
		state->map_y += state->step_y;
		state->side = RAY_SIDE_Y;
	}
}

int ray_in_bounds(t_game *game, t_ray_state *state)
{
	return (state->map_x >= 0 && state->map_x < game->map.width
		&& state->map_y >= 0 && state->map_y < game->map.height);
}

int ray_finish_hit(t_game *game, double ray_x, double ray_y,
	t_ray_state *state, t_ray_hit *hit)
{
	if (game->map.grid[state->map_y][state->map_x] == TILE_FLOOR)
		return (0);
	hit->hit = 1;
	hit->map_x = state->map_x;
	hit->map_y = state->map_y;
	hit->side = state->side;
	hit->x = game->player.pos_x + ray_x * hit->distance;
	hit->y = game->player.pos_y + ray_y * hit->distance;
	return (1);
}

int cast_ray(t_game *game, double ray_x, double ray_y, t_ray_hit *hit)
{
	t_ray_state state;
	int steps;

	hit->hit = 0;
	ray_setup(game, ray_x, ray_y, &state);
	steps = 0;
	while (steps++ < game->map.width * game->map.height * 2)
	{
		ray_advance(&state, hit);
		if (!ray_in_bounds(game, &state))
			break ;
		if (ray_finish_hit(game, ray_x, ray_y, &state, hit))
			return (1);
	}
	return (0);
}
