#include "../../cub3d.h"

#define COLOR_FOV 0x00D09020

void render_fov_ray(t_game *game, int i, t_point from)
{
	t_point hit;
	t_ray_hit result;
	double camera_x;
	double ray_x;
	double ray_y;

	camera_x = (2.0 * i / 40.0) - 1.0;
	ray_x = game->player.dir_x + game->player.plane_x * camera_x;
	ray_y = game->player.dir_y + game->player.plane_y * camera_x;
	if (cast_ray(game, ray_x, ray_y, &result))
	{
		hit.x = (int)(result.x * SIZE_TILE);
		hit.y = (int)(result.y * SIZE_TILE);
		render_line(&game->frame, from, hit, COLOR_FOV);
	}
}

int render_fov(t_game *game)
{
	int i;
	t_point from;

	from.x = (int)(game->player.pos_x * SIZE_TILE);
	from.y = (int)(game->player.pos_y * SIZE_TILE);
	i = 0;
	while (i <= 40)
		render_fov_ray(game, i++, from);
	return (0);
}

int render_tile_color(char tile)
{
	if (tile == TILE_WALL)
		return (COLOR_WALL);
	if (tile == TILE_FLOOR)
		return (COLOR_FLOOR);
	return (COLOR_VOID);
}

void render_map_cell(t_game *game, int x, int y)
{
	t_point pos;

	pos.x = x * SIZE_TILE;
	pos.y = y * SIZE_TILE;
	render_sqr(&game->frame, pos, SIZE_TILE,
		render_tile_color(game->map.grid[y][x]));
}

int render_map(t_game *game)
{
	t_point cell;

	cell.y = 0;
	while (cell.y < game->map.height)
	{
		cell.x = 0;
		while (cell.x < game->map.width)
			render_map_cell(game, cell.x++, cell.y);
		cell.y++;
	}
	return (0);
}

int render_player(t_game *game)
{
	t_point p;
	t_point center;
	t_point heading;

	center.x = (int)(game->player.pos_x * SIZE_TILE);
	center.y = (int)(game->player.pos_y * SIZE_TILE);
	p.x = center.x - SIZE_PLAYER / 2;
	p.y = center.y - SIZE_PLAYER / 2;
	render_sqr(&game->frame, p, SIZE_PLAYER, COLOR_PLAYER);
	heading.x = center.x + (int)(game->player.dir_x * SIZE_TILE * 0.7);
	heading.y = center.y + (int)(game->player.dir_y * SIZE_TILE * 0.7);
	render_line(&game->frame, center, heading, COLOR_PLAYER);
	return (0);
}
