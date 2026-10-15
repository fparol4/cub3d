#include "source/map.h"
#include "source/setup.h"

int runtime_args(int argc, char **argv)
{
	int l;
	char *m;

	m = "Usage: ./cub3d <map.cub>";
	if (argc != 2)
		return (ft_putendl_fd(m, 2), 1);
	l = ft_strlen(argv[1]);
	if (l < 4 || ft_strcmp((argv[1] + l - 4), ".cub"))
		return (ft_putendl_fd(m, 2), 1);
	return (0);
}

int runtime_cleanup(t_game *g)
{
	if (g->frame.ptr)
		mlx_destroy_image(g->mlx, g->frame.ptr);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
	map_cleanup(&g->map);
	return (0);
}


int runtime_setup(t_game *g, char *map_path)
{
	*g = (t_game) { 0 };

	if (map_parse(g, map_path))
		return (runtime_cleanup(g), 1);
	if (setup_game(g))
		return (runtime_cleanup(g), 1);
	return (0);
}

int runtime_run(t_game *g)
{
	mlx_loop(g);
	runtime_cleanup(g);
	return (0);
}

int runtime_close(t_game *g)
{
	mlx_loop_end(g->mlx);
	return (0);
}
