/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static int	_texture_load(t_game *g, t_img *img, char *path)
{
	img->ptr = mlx_xpm_file_to_image(g->graphics.mlx, path, &img->width, &img->height);
	if (!img->ptr)
		return (common_error("Cannot load wall texture"));
	img->addr = mlx_get_data_addr(img->ptr, &img->bpp, &img->stride,
			&img->endian);
	if (!img->addr || img->width <= 0 || img->height <= 0 || img->bpp != 32)
		return (common_error("Unsupported wall texture image"));
	return (0);
}

int	texture_load(t_game *g)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		if (_texture_load(g, &g->graphics.textures[i], g->map.texture_path[i]))
			return (1);
		i++;
	}
	return (0);
}

void	texture_cleanup(t_game *g)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		if (g->graphics.textures[i].ptr)
			mlx_destroy_image(g->graphics.mlx, g->graphics.textures[i].ptr);
		g->graphics.textures[i++] = (t_img){0};
	}
}

t_img	*texture_face(t_game *g, t_raycast *rc)
{
	if (rc->hit.side == RAYSIDE_X)
	{
		if (rc->ray_x > 0)
			return (&g->graphics.textures[TEX_WE]);
		return (&g->graphics.textures[TEX_EA]);
	}
	if (rc->ray_y > 0)
		return (&g->graphics.textures[TEX_NO]);
	return (&g->graphics.textures[TEX_SO]);
}
