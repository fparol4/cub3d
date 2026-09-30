#include "../../cub3d.h"

void render_column(t_img *frame, int x, int start, int end, int wall_color,
	int ceiling_color, int floor_color)
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
