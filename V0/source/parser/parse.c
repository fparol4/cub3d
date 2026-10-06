#include "../../cub3d.h"
#include "get_next_line.h"
#include <fcntl.h>

int cub_process_line(t_game *game, char *line, int *map_started,
	char ***lines, int *count, unsigned int *seen)
{
	cub_trim_line(line);
	if (!*map_started && cub_is_blank(line))
		return (1);
	if (!*map_started && cub_is_map_line(line))
		*map_started = 1;
	if (*map_started)
		return (cub_append_line(lines, count, line));
	return (cub_parse_directive(&game->map, line, seen));
}

int cub_read_lines(t_game *game, int fd, char ***lines, int *count,
	unsigned int *seen)
{
	char *line;
	int map_started;
	int line_number;
	int valid;

	map_started = 0;
	line_number = 0;
	valid = 1;
	line = get_next_line(fd);
	while (line && valid)
	{
		line_number++;
		valid = cub_process_line(game, line, &map_started,
			lines, count, seen);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	if (!valid)
		fprintf(stderr, "cub3D: invalid entry at line %d\n", line_number);
	return (valid);
}

int cub_finish_parse(t_game *game, char **lines, int count,
	unsigned int seen)
{
	if (seen != CUB_REQUIRED_FIELDS || !cub_store_grid(game, lines, count)
		|| !cub_validate_map(game))
	{
		fprintf(stderr, "cub3D: incomplete config or invalid/unenclosed map\n");
		return (0);
	}
	game->view_mode = VIEW_MINIMAP;
	game->last_frame = game_time();
	return (1);
}

int parse_cub_file(t_game *game, const char *path)
{
	int fd;
	int valid;
	int count;
	char **lines;
	unsigned int seen;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (perror(path), 1);
	lines = NULL;
	count = 0;
	seen = 0;
	valid = cub_read_lines(game, fd, &lines, &count, &seen);
	close(fd);
	if (valid)
		valid = cub_finish_parse(game, lines, count, seen);
	cub_free_lines(lines, count);
	if (!valid)
		map_destroy(&game->map);
	return (!valid);
}
