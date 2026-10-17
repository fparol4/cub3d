#include <stdlib.h>
#include "shared/cub3d.h"
#include "source/raycast.h"

static int render_px(t_img *img, t_point p, int color)
{
	char *dst;

	if (!img || !img->addr || p.x < 0 || p.x >= img->width
		|| p.y < 0 || p.y >= img->height)
		return (1);
	dst = img->addr + (p.y * img->llen + p.x * (img->bpp / 8));
	*(unsigned int *)dst = color;
	return (0);
}

static void render_line_step(t_point *point, int dx, int dy, int sx, int sy,
	int *error)
{
	int twice;

	twice = 2 * *error;
	if (twice > -dy)
	{
		*error -= dy;
		point->x += sx;
	}
	if (twice < dx)
	{
		*error += dx;
		point->y += sy;
	}
}

static void render_line(t_img *img, t_point from, t_point to, int color)
{
	int dx;
	int dy;
	int sx;
	int sy;
	int error;
	t_point point;

	dx = abs(to.x - from.x);
	dy = abs(to.y - from.y);
	sx = (from.x < to.x) - (from.x > to.x);
	sy = (from.y < to.y) - (from.y > to.y);
	error = dx - dy;
	point = from;
	while (1)
	{
		render_px(img, point, color);
		if (point.x == to.x && point.y == to.y)
			break ;
		render_line_step(&point, dx, dy, sx, sy, &error);
	}
}

static int render_sqr(t_img *img, t_point point, int size, int color)
{
	t_point pixel;

	pixel.y = point.y;
	while (pixel.y < point.y + size)
	{
		pixel.x = point.x;
		while (pixel.x < point.x + size)
		{
			render_px(img, pixel, color);
			pixel.x++;
		}
		pixel.y++;
	}
	return (0);
}

static void render_column(t_img *frame, int x, int start, int end,
	int wall_color, int ceiling_color, int floor_color)
{
	t_point pixel;

	pixel.x = x;
	pixel.y = 0;
	while (pixel.y < start)
	{
		render_px(frame, pixel, ceiling_color);
		pixel.y++;
	}
	while (pixel.y < end)
	{
		render_px(frame, pixel, wall_color);
		pixel.y++;
	}
	while (pixel.y < frame->height)
	{
		render_px(frame, pixel, floor_color);
		pixel.y++;
	}
}

static int render_tile_color(char tile)
{
	if (tile == T_WALL)
		return (COLOR_WALL);
	if (tile == T_FLOOR)
		return (COLOR_FLOOR);
	return (COLOR_VOID);
}

static void render_map_cell(t_game *game, int x, int y)
{
	t_point pos;

	pos.x = x * S_TILE;
	pos.y = y * S_TILE;
	render_sqr(&game->frame, pos, S_TILE,
		render_tile_color(game->map.grid[y][x]));
}

static int render_map(t_game *game)
{
	t_point cell;

	cell.y = 0;
	while (cell.y < game->map.height)
	{
		cell.x = 0;
		while (cell.x < game->map.width)
		{
			render_map_cell(game, cell.x, cell.y);
			cell.x++;
		}
		cell.y++;
	}
	return (0);
}

static void render_fov_ray(t_game *game, int i, t_point from)
{
	t_raycast ray;
	t_point hit;
	double camera_x;

	ray = (t_raycast){0};
	camera_x = (2.0 * i / (RENDER_FOV_RAYS - 1)) - 1.0;
	ray.ray_x = game->player.dir_x + game->player.plane_x * camera_x;
	ray.ray_y = game->player.dir_y + game->player.plane_y * camera_x;
	if (raycast_cast(game, &ray))
	{
		hit.x = (int)(ray.hit.x * S_TILE);
		hit.y = (int)(ray.hit.y * S_TILE);
		render_line(&game->frame, from, hit, COLOR_FOV);
	}
}

static int render_fov(t_game *game)
{
	int i;
	t_point from;

	from.x = (int)(game->player.pos_x * S_TILE);
	from.y = (int)(game->player.pos_y * S_TILE);
	i = 0;
	while (i < RENDER_FOV_RAYS)
	{
		render_fov_ray(game, i, from);
		i++;
	}
	return (0);
}

static int render_player(t_game *game)
{
	t_point pos;
	t_point center;
	t_point heading;

	center.x = (int)(game->player.pos_x * S_TILE);
	center.y = (int)(game->player.pos_y * S_TILE);
	pos.x = center.x - S_PLAYER / 2;
	pos.y = center.y - S_PLAYER / 2;
	render_sqr(&game->frame, pos, S_PLAYER, COLOR_PLAYER);
	heading.x = center.x + (int)(game->player.dir_x * S_TILE * 0.7);
	heading.y = center.y + (int)(game->player.dir_y * S_TILE * 0.7);
	render_line(&game->frame, center, heading, COLOR_PLAYER);
	return (0);
}

int render_2D(t_game *game)
{
	render_map(game);
	render_fov(game);
	render_player(game);
	return (0);
}

static double render_camera_x(int x, int width)
{
	return (2.0 * x / width - 1.0);
}

static void render_wall_column(t_game *game, int x, t_ray_hit *hit)
{
	int line_height;
	int draw_start;
	int draw_end;
	int color;
	double distance;

	distance = hit->distance;
	if (distance < RENDER_MIN_DISTANCE)
		distance = RENDER_MIN_DISTANCE;
	line_height = (int)(game->frame.height / distance);
	draw_start = game->frame.height / 2 - line_height / 2;
	draw_end = game->frame.height / 2 + line_height / 2;
	if (draw_start < 0)
		draw_start = 0;
	if (draw_end > game->frame.height)
		draw_end = game->frame.height;
	color = COLOR_WALL_3D;
	if (hit->side == RAYSIDE_Y)
		color = COLOR_WALL_DARK;
	render_column(&game->frame, x, draw_start, draw_end, color,
		game->map.color_ceiling, game->map.color_floor);
}

static void render_3d_column(t_game *game, int x)
{
	t_raycast ray;
	double camera_x;

	ray = (t_raycast){0};
	camera_x = render_camera_x(x, game->frame.width);
	ray.ray_x = game->player.dir_x + game->player.plane_x * camera_x;
	ray.ray_y = game->player.dir_y + game->player.plane_y * camera_x;
	if (raycast_cast(game, &ray))
		render_wall_column(game, x, &ray.hit);
	else
		render_column(&game->frame, x, game->frame.height / 2,
			game->frame.height / 2, COLOR_WALL_3D,
			game->map.color_ceiling, game->map.color_floor);
}

int render_3D(t_game *game)
{
	int x;

	x = 0;
	while (x < game->frame.width)
	{
		render_3d_column(game, x);
		x++;
	}
	return (0);
}
