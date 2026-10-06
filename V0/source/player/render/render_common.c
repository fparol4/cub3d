#include "../../cub3d.h"

void render_line(t_img *img, t_point from, t_point to, int color)
{
	int dx;
	int dy;
	int sx;
	int sy;
	int error;
	t_point p;

	dx = abs(to.x - from.x);
	dy = abs(to.y - from.y);
	sx = (from.x < to.x) - (from.x > to.x);
	sy = (from.y < to.y) - (from.y > to.y);
	error = dx - dy;
	p = from;
	while (1)
	{
		render_px(img, p, color);
		if (p.x == to.x && p.y == to.y)
			break ;
		render_line_step(&p, dx, dy, sx, sy, &error);
	}
}

void render_line_step(t_point *point, int dx, int dy, int sx, int sy,
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

int render_px(t_img *img, t_point p, int color)
{
	char *dst;

	if (p.x < 0 || p.x >= img->width
		|| p.y < 0 || p.y >= img->height)
		return (1);
	dst = img->addr + (p.y * img->llen + p.x * (img->bpp / 8));
	*(unsigned int *)dst = color;
	return (0);
}

int render_sqr(t_img *img, t_point point, int size, int color)
{
	t_point p;

	p.y = point.y;
	while (p.y < point.y + size)
	{
		p.x = point.x;
		while (p.x < point.x + size)
		{
			render_px(img, p, color);
			p.x++;
		}
		p.y++;
	}
	return (0);
}
