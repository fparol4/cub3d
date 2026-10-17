/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "source/render.h"

static void	_render_line_step(t_point *p, t_line *line)
{
	int	twice;

	twice = 2 * line->error;
	if (twice > -line->dy)
	{
		line->error -= line->dy;
		p->x += line->sx;
	}
	if (twice < line->dx)
	{
		line->error += line->dx;
		p->y += line->sy;
	}
}

void	render_px(t_img *img, t_point p, int color)
{
	unsigned int	*dst;

	if (p.x < 0 || p.x >= img->width || p.y < 0 || p.y >= img->height)
		return ;
	dst = (unsigned int *)(img->addr + p.y * img->llen
			+ p.x * (img->bpp / 8));
	*dst = (unsigned int)color;
}

void	render_line(t_img *img, t_point from, t_point to, int color)
{
	t_line	line;

	line.dx = ft_abs(to.x - from.x);
	line.dy = ft_abs(to.y - from.y);
	line.sx = (from.x < to.x) - (from.x > to.x);
	line.sy = (from.y < to.y) - (from.y > to.y);
	line.error = line.dx - line.dy;
	while (1)
	{
		render_px(img, from, color);
		if (from.x == to.x && from.y == to.y)
			break ;
		_render_line_step(&from, &line);
	}
}

void	render_rect(t_img *img, t_point from, t_point to, int color)
{
	t_point	pixel;

	pixel.y = from.y;
	while (pixel.y < to.y)
	{
		pixel.x = from.x;
		while (pixel.x < to.x)
		{
			render_px(img, pixel, color);
			pixel.x++;
		}
		pixel.y++;
	}
}

t_point	render_map_point(t_game *g, t_viewport *v, double x, double y)
{
	t_point	p;

	p.x = v->x + (int)(x * v->width / g->map.width);
	p.y = v->y + (int)(y * v->height / g->map.height);
	return (p);
}
