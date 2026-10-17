#include "source/map.h"
#include <string.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	t_game	game;
	char	*path;

	if (argc == 3 && strcmp(argv[1], "--textures") == 0)
		path = argv[2];
	else if (argc == 2)
		path = argv[1];
	else
		return (1);
	game = (t_game){0};
	if (map_parse(&game, path))
		return (1);
	if (argc == 3 && (access(game.map.no_texture, R_OK)
		|| access(game.map.so_texture, R_OK)
		|| access(game.map.we_texture, R_OK)
		|| access(game.map.ea_texture, R_OK)))
		return (map_cleanup(&game.map), 1);
	map_cleanup(&game.map);
	return (0);
}
