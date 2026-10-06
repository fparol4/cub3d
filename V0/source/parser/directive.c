#include "../../cub3d.h"
#include "libft.h"
#include <ctype.h>
#include <string.h>

int cub_parse_component(const char **value, long *component, int comma)
{
	char *end;

	while (**value == ' ' || **value == '\t')
		(*value)++;
	if (!isdigit((unsigned char)**value))
		return (0);
	*component = strtol(*value, &end, 10);
	if (*component > 255)
		return (0);
	*value = end;
	while (**value == ' ' || **value == '\t')
		(*value)++;
	if (comma && *(*value)++ != ',')
		return (0);
	return (1);
}

int cub_parse_color(const char *value, int *color)
{
	long rgb[3];
	int i;

	i = 0;
	while (i < 3)
	{
		if (!cub_parse_component(&value, &rgb[i], i < 2))
			return (0);
		i++;
	}
	while (*value == ' ' || *value == '\t')
		value++;
	if (*value)
		return (0);
	*color = (int)((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
	return (1);
}

unsigned int cub_directive_field(const char *key, size_t len, t_map *map,
	char ***texture)
{
	*texture = NULL;
	if (len == 1 && key[0] == 'F')
		return (CUB_FLOOR);
	if (len == 1 && key[0] == 'C')
		return (CUB_CEILING);
	return (cub_texture_field(key, len, map, texture));
}

unsigned int cub_texture_field(const char *key, size_t len, t_map *map,
	char ***texture)
{
	if (len == 2 && !strncmp(key, "NO", 2))
	{
		*texture = &map->no_texture;
		return (CUB_NO);
	}
	if (len == 2 && !strncmp(key, "SO", 2))
	{
		*texture = &map->so_texture;
		return (CUB_SO);
	}
	if (len == 2 && !strncmp(key, "WE", 2))
	{
		*texture = &map->we_texture;
		return (CUB_WE);
	}
	if (len == 2 && !strncmp(key, "EA", 2))
	{
		*texture = &map->ea_texture;
		return (CUB_EA);
	}
	return (0);
}

int cub_store_directive(t_map *map, unsigned int field, const char *value,
	char **texture)
{
	size_t len;

	if (texture)
	{
		len = ft_strlen(value);
		while (len && (value[len - 1] == ' ' || value[len - 1] == '\t'))
			len--;
		*texture = ft_substr(value, 0, len);
		return (*texture != NULL);
	}
	if (field == CUB_FLOOR)
		return (cub_parse_color(value, &map->floor_color));
	return (cub_parse_color(value, &map->ceiling_color));
}

int cub_parse_directive(t_map *map, const char *line, unsigned int *seen)
{
	const char *key;
	const char *value;
	unsigned int field;
	char **texture;

	while (*line == ' ' || *line == '\t')
		line++;
	key = line;
	while (*line && *line != ' ' && *line != '\t')
		line++;
	field = cub_directive_field(key, line - key, map, &texture);
	while (*line == ' ' || *line == '\t')
		line++;
	value = line;
	if (!field || !*value || (*seen & field))
		return (0);
	if (!cub_store_directive(map, field, value, texture))
		return (0);
	*seen |= field;

	return (1);
}
