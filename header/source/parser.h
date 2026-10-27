/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "core/struct.h"
# include <stddef.h>

typedef struct s_parser_data
{
	t_map			map;
	int				player_x;
	int				player_y;
	char			player_dir;
	unsigned int	fields;
	int				map_started;
}	t_parser_data;

typedef struct s_parser_flood
{
	t_parser_data	*data;
	char		*visited;
	t_point		*queue;
	size_t		tail;
	size_t		capacity;
}	t_parser_flood;

void	parser_cleanup(t_map *m);
int		parser_controller(t_game *g, char *map_path);
int		parser_split_archive(t_parser_data *data, const char *path);
int		parser_parse_config(t_parser_data *data, char *line);
int		parser_parse_color(const char *value, int *color);
int		parser_valid_map(t_parser_data *data);
int		parser_validate_map_connectivity(t_parser_data *data);
int		parser_is_walkable(char cell);
int		parser_is_player(char cell);
char	parser_map_at(t_parser_data *data, int x, int y);
int		parser_rectangularize(t_map *map);
size_t	parser_count_walkable_cells(t_parser_data *data);

#endif
