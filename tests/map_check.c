#include "source/parser.h"
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
	if (parser_controller(&game, path))
		return (1);
	if (argc == 3 && (access(game.map.texture_path[TEX_NO], R_OK)
		|| access(game.map.texture_path[TEX_SO], R_OK)
		|| access(game.map.texture_path[TEX_WE], R_OK)
		|| access(game.map.texture_path[TEX_EA], R_OK)))
		return (parser_cleanup(&game.map), 1);
	parser_cleanup(&game.map);
	return (0);
}
