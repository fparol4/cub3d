/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

int	runtime_args(int argc, char **argv)
{
	size_t	length;
	char	*message;

	message = "Usage: ./cub3d <map.cub>";
	if (argc != 2)
		return (common_error(message));
	length = ft_strlen(argv[1]);
	if (length < 4 || ft_strcmp(argv[1] + length - 4, ".cub"))
		return (common_error(message));
	return (0);
}

void	runtime_cleanup(t_game *g)
{
	texture_cleanup(g);
	if (g->graphics.frame.ptr)
		mlx_destroy_image(g->graphics.mlx, g->graphics.frame.ptr);
	if (g->graphics.window)
		mlx_destroy_window(g->graphics.mlx, g->graphics.window);
	if (g->graphics.mlx)
	{
		mlx_destroy_display(g->graphics.mlx);
		free(g->graphics.mlx);
	}
	parser_cleanup(&g->map);
	g->graphics.frame = (t_img){0};
	g->graphics.window = NULL;
	g->graphics.mlx = NULL;
}

int	runtime_setup(t_game *g, char *map_path)
{
	*g = (t_game){0};
	if (parser_controller(g, map_path) || setup_game(g))
		return (runtime_cleanup(g), 1);
	g->state.needs_redraw = 1;
	return (0);
}

int	runtime_run(t_game *g)
{
	mlx_loop(g->graphics.mlx);
	runtime_cleanup(g);
	return (g->state.exit_status);
}

int	runtime_close(t_game *g)
{
	mlx_loop_end(g->graphics.mlx);
	return (0);
}
