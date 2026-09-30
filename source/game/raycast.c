#include "game.h"
#include <math.h>

int cast_ray(t_game *game, double ray_x, double ray_y, t_ray_hit *hit)
{
	int map_x;
	int map_y;
	int step_x;
	int step_y;
	t_ray_side side;
	int steps;
	double delta_x;
	double delta_y;
	double side_x;
	double side_y;

	map_x = (int)floor(game->player.pos_x);
	map_y = (int)floor(game->player.pos_y);
	delta_x = (ray_x == 0.0) ? 1e30 : fabs(1.0 / ray_x);
	delta_y = (ray_y == 0.0) ? 1e30 : fabs(1.0 / ray_y);
	step_x = (ray_x < 0.0) ? -1 : 1;
	step_y = (ray_y < 0.0) ? -1 : 1;
	if (ray_x < 0.0)
		side_x = (game->player.pos_x - map_x) * delta_x;
	else
		side_x = (map_x + 1.0 - game->player.pos_x) * delta_x;
	if (ray_y < 0.0)
		side_y = (game->player.pos_y - map_y) * delta_y;
	else
		side_y = (map_y + 1.0 - game->player.pos_y) * delta_y;
	steps = 0;
	while (steps++ < game->map.width * game->map.height * 2)
	{
		if (side_x < side_y)
		{
			hit->distance = side_x;
			side_x += delta_x;
			map_x += step_x;
			side = RAY_SIDE_X;
		}
		else
		{
			hit->distance = side_y;
			side_y += delta_y;
			map_y += step_y;
			side = RAY_SIDE_Y;
		}
		if (map_x < 0 || map_x >= game->map.width
			|| map_y < 0 || map_y >= game->map.height)
			break ;
		if (game->map.grid[map_y][map_x] == TILE_WALL)
		{
			hit->hit = 1;
			hit->map_x = map_x;
			hit->map_y = map_y;
			hit->side = side;
			hit->x = game->player.pos_x + ray_x * hit->distance;
			hit->y = game->player.pos_y + ray_y * hit->distance;
			return (1);
		}
	}
	hit->hit = 0;
	return (0);
}
