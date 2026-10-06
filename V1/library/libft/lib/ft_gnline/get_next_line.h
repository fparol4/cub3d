/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <fcntl.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
void	ft_read_line(int fd, char **full_string);
char	*ft_get_line(char *line);
char	*ft_new_line(char *full_string);
char	*gnl_strjoin(char const *string_line, char *buffer);
size_t	gnl_strlen(const char *string);
char	*gnl_strchr(const char *string, int c);
char	*gnl_substr(char const *full_string, unsigned int start, size_t len);
void	*gnl_calloc(size_t nmemb, size_t size);
void	gnl_bzero(void *s, size_t n);
int		check_read(int read_bytes, char ***full_string);

#endif
