/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include "core/struct.h"
# include <stddef.h>

typedef struct s_map_data
{
	t_map			map;
	int				player_x;
	int				player_y;
	char			player_dir;
	unsigned int	fields;
	int				map_started;
}	t_map_data;

typedef struct s_map_flood
{
	t_map_data	*data;
	char		*visited;
	t_point		*queue;
	size_t		tail;
	size_t		capacity;
}	t_map_flood;

void	map_cleanup(t_map *m);
int		map_parse(t_game *g, char *map_path);
int		map_read_archive(t_map_data *data, const char *path);
int		map_parse_config(t_map_data *data, char *line);
int		map_parse_color(const char *value, int *color);
int		map_validate(t_map_data *data);
int		map_connectivity_validate(t_map_data *data);
int		map_is_walkable(char cell);
int		map_is_player(char cell);
char	map_cell_at(t_map_data *data, int x, int y);
int		map_rectangularize(t_map *map);
size_t	map_count_walkable(t_map_data *data);

#endif
