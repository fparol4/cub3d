/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 17:02:50 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 17:02:50 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "core/struct.h"
# include <stddef.h>
# include <sys/types.h>

typedef struct s_parser_data
{
	t_map			map;
	int				player_x;
	int				player_y;
	char			player_dir;
	unsigned int	fields;
	int				map_started;
	int				map_finished;
}					t_parser_data;

typedef struct s_parser_reader
{
	int					fd;
	char				buffer[4096];
	ssize_t				size;
	size_t				offset;
}					t_parser_reader;

typedef struct s_parser_line
{
	char				*text;
	size_t				length;
	size_t				capacity;
}					t_parser_line;

void				parser_cleanup(t_map *map);
int					parser_load_map(t_game *g, char *map_path);
int					parser_read_file(t_parser_data *data, const char *path);
int					parser_store_map_line(t_parser_data *data, char *line);
int					parser_parse_config(t_parser_data *data, char *line);
int					parser_parse_color(const char *value, int *color);
int					parser_validate_map(t_parser_data *data);
int					parser_is_walkable(char cell);
int					parser_is_player(char cell);
char				parser_map_at(t_parser_data *data, int x, int y);
int					parser_rectangularize(t_map *map);

int					parser_next_line(t_parser_reader *reader, char **line);
int					parser_process_line(t_parser_data *data, char *line);

#endif
