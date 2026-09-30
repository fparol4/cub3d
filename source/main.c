#include "../cub3d.h"
#include "libft.h"
#include <string.h>

int main(int argc, char **argv)
{
	t_game game;
	size_t len;

	if (argc != 2)
	{
		ft_putendl_fd("Usage: ./cub3d <map.cub>", 2);
		return (1);
	}
	len = strlen(argv[1]);
	if (len < 4 || strcmp(argv[1] + len - 4, ".cub"))
	{
		ft_putendl_fd("Usage: ./cub3d <map.cub>", 2);
		return (1);
	}
	if (game_setup(&game, argv[1]))
		return (1);
	mlx_loop(game.mlx);
	game_cleanup(&game);
	return (0);
}
