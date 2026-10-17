/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "mlx.h"
#include "source/common.h"
#include "source/texture.h"

static int	_texture_load(t_game *g, t_img *img, char *path)
{
	img->ptr = mlx_xpm_file_to_image(g->mlx, path, &img->width, &img->height);
	if (!img->ptr)
		return (common_error("Cannot load wall texture"));
	img->addr = mlx_get_data_addr(img->ptr, &img->bpp, &img->llen,
			&img->endian);
	if (!img->addr || img->width <= 0 || img->height <= 0 || img->bpp != 32)
		return (common_error("Unsupported wall texture image"));
	return (0);
}

int	texture_load(t_game *g)
{
	if (_texture_load(g, &g->texture[TEX_NO], g->map.no_texture)
		|| _texture_load(g, &g->texture[TEX_SO], g->map.so_texture)
		|| _texture_load(g, &g->texture[TEX_WE], g->map.we_texture)
		|| _texture_load(g, &g->texture[TEX_EA], g->map.ea_texture))
		return (1);
	return (0);
}

void	texture_cleanup(t_game *g)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		if (g->texture[i].ptr)
			mlx_destroy_image(g->mlx, g->texture[i].ptr);
		g->texture[i++] = (t_img){0};
	}
}

t_img	*texture_face(t_game *g, t_raycast *rc)
{
	if (rc->hit.side == RAYSIDE_X)
	{
		if (rc->ray_x > 0)
			return (&g->texture[TEX_WE]);
		return (&g->texture[TEX_EA]);
	}
	if (rc->ray_y > 0)
		return (&g->texture[TEX_NO]);
	return (&g->texture[TEX_SO]);
}

int	texture_pixel(t_img *img, int x, int y)
{
	int	color;

	color = 0;
	ft_memcpy(&color, img->addr + y * img->llen + x * (img->bpp / 8),
		sizeof(color));
	return (color);
}
