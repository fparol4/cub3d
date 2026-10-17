#include <stdlib.h>
#include "core/const.h"
#include "source/render.h"
#include "source/raycast.h"

static int _render_px(t_img *img, t_point p, int color)
{
	char *dst;

	if (!img || !img->addr || p.x < 0 || p.x >= img->width
		|| p.y < 0 || p.y >= img->height)
		return (1);
	dst = img->addr + (p.y * img->llen + p.x * (img->bpp / 8));
	*(unsigned int *)dst = color;
	return (0);
}

static void _render_line_step(t_point *point, int dx, int dy, int sx, int sy,
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

static void _render_line(t_img *img, t_point from, t_point to, int color)
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
		_render_px(img, point, color);
		if (point.x == to.x && point.y == to.y)
			break ;
		_render_line_step(&point, dx, dy, sx, sy, &error);
	}
}

static int _render_sqr(t_img *img, t_point point, int size, int color)
{
	t_point pixel;

	pixel.y = point.y;
	while (pixel.y < point.y + size)
	{
		pixel.x = point.x;
		while (pixel.x < point.x + size)
		{
			_render_px(img, pixel, color);
			pixel.x++;
		}
		pixel.y++;
	}
	return (0);
}

static void _render_column(t_img *frame, int x, int start, int end,
	int wall_color, int ceiling_color, int floor_color)
{
	t_point pixel;

	pixel.x = x;
	pixel.y = 0;
	while (pixel.y < start)
	{
		_render_px(frame, pixel, ceiling_color);
		pixel.y++;
	}
	while (pixel.y < end)
	{
		_render_px(frame, pixel, wall_color);
		pixel.y++;
	}
	while (pixel.y < frame->height)
	{
		_render_px(frame, pixel, floor_color);
		pixel.y++;
	}
}

static int _render_tile_color(char tile)
{
	if (tile == T_WALL)
		return (COLOR_WALL);
	if (tile == T_FLOOR)
		return (COLOR_FLOOR);
	return (COLOR_VOID);
}

static void _render_map_cell(t_game *g, int x, int y)
{
	t_point pos;

	pos.x = x * S_TILE;
	pos.y = y * S_TILE;
	_render_sqr(&g->frame, pos, S_TILE,
		_render_tile_color(g->map.grid[y][x]));
}

static int _render_map(t_game *g)
{
	t_point cell;

	cell.y = 0;
	while (cell.y < g->map.height)
	{
		cell.x = 0;
		while (cell.x < g->map.width)
		{
			_render_map_cell(g, cell.x, cell.y);
			cell.x++;
		}
		cell.y++;
	}
	return (0);
}

static void _render_fov_ray(t_game *g, int i, t_point from)
{
	t_raycast ray;
	t_point hit;
	double camera_x;

	ray = (t_raycast){0};
	camera_x = (2.0 * i / (RENDER_FOV_RAYS - 1)) - 1.0;
	ray.ray_x = g->player.dir_x + g->player.plane_x * camera_x;
	ray.ray_y = g->player.dir_y + g->player.plane_y * camera_x;
	if (raycast_cast(g, &ray))
	{
		hit.x = (int)(ray.hit.x * S_TILE);
		hit.y = (int)(ray.hit.y * S_TILE);
		_render_line(&g->frame, from, hit, COLOR_FOV);
	}
}

static int _render_fov(t_game *g)
{
	int i;
	t_point from;

	from.x = (int)(g->player.pos_x * S_TILE);
	from.y = (int)(g->player.pos_y * S_TILE);
	i = 0;
	while (i < RENDER_FOV_RAYS)
	{
		_render_fov_ray(g, i, from);
		i++;
	}
	return (0);
}

static int _render_player(t_game *g)
{
	t_point pos;
	t_point center;
	t_point heading;

	center.x = (int)(g->player.pos_x * S_TILE);
	center.y = (int)(g->player.pos_y * S_TILE);
	pos.x = center.x - S_PLAYER / 2;
	pos.y = center.y - S_PLAYER / 2;
	_render_sqr(&g->frame, pos, S_PLAYER, COLOR_PLAYER);
	heading.x = center.x + (int)(g->player.dir_x * S_TILE * 0.7);
	heading.y = center.y + (int)(g->player.dir_y * S_TILE * 0.7);
	_render_line(&g->frame, center, heading, COLOR_PLAYER);
	return (0);
}

int render_2D(t_game *g)
{
	_render_map(g);
	_render_fov(g);
	_render_player(g);
	return (0);
}

static double _render_camera_x(int x, int width)
{
	return (2.0 * x / width - 1.0);
}

static void _render_wall_column(t_game *g, int x, t_ray_hit *hit)
{
	int line_height;
	int draw_start;
	int draw_end;
	int color;
	double distance;

	distance = hit->distance;
	if (distance < RENDER_MIN_DISTANCE)
		distance = RENDER_MIN_DISTANCE;
	line_height = (int)(g->frame.height / distance);
	draw_start = g->frame.height / 2 - line_height / 2;
	draw_end = g->frame.height / 2 + line_height / 2;
	if (draw_start < 0)
		draw_start = 0;
	if (draw_end > g->frame.height)
		draw_end = g->frame.height;
	color = COLOR_WALL_3D;
	if (hit->side == RAYSIDE_Y)
		color = COLOR_WALL_DARK;
	_render_column(&g->frame, x, draw_start, draw_end, color,
		g->map.color_ceiling, g->map.color_floor);
}

static void _render_3d_column(t_game *g, int x)
{
	t_raycast ray;
	double camera_x;

	ray = (t_raycast){0};
	camera_x = _render_camera_x(x, g->frame.width);
	ray.ray_x = g->player.dir_x + g->player.plane_x * camera_x;
	ray.ray_y = g->player.dir_y + g->player.plane_y * camera_x;
	if (raycast_cast(g, &ray))
		_render_wall_column(g, x, &ray.hit);
	else
		_render_column(&g->frame, x, g->frame.height / 2,
			g->frame.height / 2, COLOR_WALL_3D,
			g->map.color_ceiling, g->map.color_floor);
}

int render_3D(t_game *g)
{
	int x;

	x = 0;
	while (x < g->frame.width)
	{
		_render_3d_column(g, x);
		x++;
	}
	return (0);
}
