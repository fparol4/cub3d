#include "../../cub3d.h"
#include "libft.h"
#include <string.h>

char *cub_trim_line(char *line)
{
	size_t len;

	len = ft_strlen(line);
	while (len && (line[len - 1] == '\n' || line[len - 1] == '\r'))
		line[--len] = '\0';
	return (line);
}

int cub_is_blank(const char *line)
{
	while (*line == ' ' || *line == '\t')
		line++;
	return (*line == '\0');
}

int cub_is_valid_tile(char tile)
{
	return (tile == ' ' || tile == TILE_FLOOR || tile == TILE_WALL
		|| cub_is_spawn(tile));
}

int cub_is_spawn(char tile)
{
	return (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W');
}

int cub_is_walkable(char tile)
{
	return (tile == TILE_FLOOR || cub_is_spawn(tile));
}

int cub_is_map_line(const char *line)
{
	int has_tile;

	has_tile = 0;
	while (*line)
	{
		if (!cub_is_valid_tile(*line))
			return (0);
		if (*line++ != ' ')
			has_tile = 1;
	}
	return (has_tile);
}

int cub_append_line(char ***lines, int *count, const char *line)
{
	char **new_lines;
	char *copy;

	copy = ft_strdup(line);
	if (!copy)
		return (0);
	new_lines = realloc(*lines, sizeof(char *) * (*count + 1));
	if (!new_lines)
	{
		free(copy);
		return (0);
	}
	new_lines[*count] = copy;
	*lines = new_lines;
	(*count)++;
	return (1);
}

int cub_map_width(char **lines, int count)
{
	int width;
	int i;

	width = 0;
	i = 0;
	while (i < count)
	{
		if ((int)ft_strlen(lines[i]) > width)
			width = ft_strlen(lines[i]);
		i++;
	}
	return (width);
}

int cub_copy_grid_row(char *destination, const char *source)
{
	int i;

	i = 0;
	while (source[i])
	{
		if (!cub_is_valid_tile(source[i]))
			return (0);
		destination[i] = source[i];
		i++;
	}
	return (1);
}

int cub_store_grid(t_game *game, char **lines, int count)
{
	int i;
	int width;

	while (count > 0 && cub_is_blank(lines[count - 1]))
		count--;
	if (!count)
		return (0);
	width = cub_map_width(lines, count);
	game->map.grid = ft_calloc(count + 1, sizeof(char *));
	if (!game->map.grid)
		return (0);
	game->map.width = width;
	game->map.height = count;
	i = 0;
	while (i < count)
		if (!cub_alloc_grid_row(game, lines, i++, width))
			return (0);
	return (1);
}

int cub_alloc_grid_row(t_game *game, char **lines, int row, int width)
{
	game->map.grid[row] = ft_calloc(width + 1, sizeof(char));
	if (!game->map.grid[row])
		return (0);
	memset(game->map.grid[row], ' ', width);
	return (cub_copy_grid_row(game->map.grid[row], lines[row]));
}

void cub_set_player(t_game *game, char tile, int x, int y)
{
	game->player.pos_x = x + 0.5;
	game->player.pos_y = y + 0.5;
	game->player.dir_x = (tile == 'E') - (tile == 'W');
	game->player.dir_y = (tile == 'S') - (tile == 'N');
	game->player.plane_x = -game->player.dir_y;
	game->player.plane_y = game->player.dir_x;
	game->map.grid[y][x] = TILE_FLOOR;
}

int cub_find_player(t_game *game)
{
	int x;
	int y;
	int count;

	count = 0;
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (cub_is_spawn(game->map.grid[y][x]))
			{
				cub_set_player(game, game->map.grid[y][x], x, y);
				count++;
			}
			x++;
		}
		y++;
	}
	return (count == 1);
}

int cub_cell_enclosed(t_game *game, int x, int y)
{
	int dx;
	int dy;

	dy = -1;
	while (dy <= 1)
	{
		dx = -1;
		while (dx <= 1)
		{
			if (x + dx < 0 || x + dx >= game->map.width
				|| y + dy < 0 || y + dy >= game->map.height
				|| game->map.grid[y + dy][x + dx] == ' ')
				return (0);
			dx++;
		}
		dy++;
	}
	return (1);
}

int cub_check_enclosure(t_game *game)
{
	int x;
	int y;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (cub_is_walkable(game->map.grid[y][x])
				&& !cub_cell_enclosed(game, x, y))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

int cub_validate_map(t_game *game)
{
	if (game->map.width < 3 || game->map.height < 3)
		return (0);
	return (cub_find_player(game) && cub_check_enclosure(game));
}

void cub_free_lines(char **lines, int count)
{
	int i;

	i = 0;
	while (i < count)
		free(lines[i++]);
	free(lines);
}

void map_destroy(t_map *map)
{
	int i;

	i = 0;
	while (map->grid && i < map->height)
		free(map->grid[i++]);
	free(map->grid);
	free(map->no_texture);
	free(map->so_texture);
	free(map->we_texture);
	free(map->ea_texture);
	memset(map, 0, sizeof(*map));
}
