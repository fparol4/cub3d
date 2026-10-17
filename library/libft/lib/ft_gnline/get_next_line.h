/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:06 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:06 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <stddef.h>

typedef struct s_gnl_state
{
	int					fd;
	char				*data;
	size_t				length;
	size_t				capacity;
	struct s_gnl_state	*next;
}						t_gnl_state;

char					*ft_get_next_line(int fd);
int						gnl_append(t_gnl_state *state, const char *chunk,
							size_t size);
size_t					gnl_line_size(t_gnl_state *state);
char					*gnl_take_line(t_gnl_state *state, size_t line_size);
void					gnl_remove(t_gnl_state **states, int fd);

#endif
