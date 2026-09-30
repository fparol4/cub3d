#include "../../cub3d.h"

double render_camera_x(int x, int width)
{
	return (2.0 * x / width - 1.0);
}

void render_wall_column(t_game *game, int x, t_ray_hit *hit)
{
	int line_height;
	int draw_start;
	int draw_end;
	int color;
	double distance;

	distance = hit->distance;
	if (distance < 0.05)
		distance = 0.05;
	line_height = (int)(game->frame.height / distance);
	draw_start = game->frame.height / 2 - line_height / 2;
	draw_end = game->frame.height / 2 + line_height / 2;
	if (draw_start < 0)
		draw_start = 0;
	if (draw_end > game->frame.height)
		draw_end = game->frame.height;
	color = (hit->side == RAY_SIDE_Y) ? COLOR_WALL_DARK : COLOR_WALL_3D;
	render_column(&game->frame, x, draw_start, draw_end, color,
		game->map.ceiling_color, game->map.floor_color);
}

void render_3d_column(t_game *game, int x)
{
	t_ray_hit hit;
	double camera_x;
	double ray_x;
	double ray_y;

	camera_x = render_camera_x(x, game->frame.width);
	ray_x = game->player.dir_x + game->player.plane_x * camera_x;
	ray_y = game->player.dir_y + game->player.plane_y * camera_x;
	if (cast_ray(game, ray_x, ray_y, &hit))
		render_wall_column(game, x, &hit);
	else
		render_column(&game->frame, x, game->frame.height / 2,
			game->frame.height / 2, COLOR_WALL_3D,
			game->map.ceiling_color, game->map.floor_color);
}

int render_3d(t_game *game)
{
	int x;

	x = 0;
	while (x < game->frame.width)
		render_3d_column(game, x++);
	return (0);
}
