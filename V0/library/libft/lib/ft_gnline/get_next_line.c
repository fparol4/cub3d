/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*full_string;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(full_string);
		full_string = NULL;
		return (NULL);
	}
	ft_read_line(fd, &full_string);
	if (!full_string)
		return (NULL);
	else if (!full_string[0])
	{
		free(full_string);
		full_string = NULL;
		return (NULL);
	}
	line = ft_get_line(full_string);
	full_string = ft_new_line(full_string);
	return (line);
}

void	ft_read_line(int fd, char **full_string)
{
	char	*buffer;
	int		read_bytes;
	char	*temp;

	read_bytes = 1;
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return ;
	while (!(gnl_strchr(*full_string, '\n')) && read_bytes)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (!(check_read(read_bytes, &full_string)))
		{
			free(buffer);
			return ;
		}
		buffer[read_bytes] = '\0';
		temp = *full_string;
		*full_string = gnl_strjoin(*full_string, buffer);
		free(temp);
	}
	free(buffer);
}

char	*ft_get_line(char *line)
{
	size_t	count_bytes;
	char	*string_line;

	count_bytes = 0;
	while (line[count_bytes] != '\n' && line[count_bytes] != '\0')
		count_bytes++;
	string_line = malloc(count_bytes + 2);
	if (!string_line)
		return (NULL);
	count_bytes = 0;
	while (line[count_bytes] != '\n' && line[count_bytes] != '\0')
	{
		string_line[count_bytes] = line[count_bytes];
		count_bytes++;
	}
	if (line[count_bytes] == '\n')
		string_line[count_bytes++] = '\n';
	string_line[count_bytes] = '\0';
	return (string_line);
}

char	*ft_new_line(char *full_string)
{
	size_t	count_bytes;
	size_t	count_remainder;
	size_t	fill_remainder;
	char	*buffer_remainder;

	buffer_remainder = NULL;
	if (!full_string)
		return (NULL);
	count_bytes = 0;
	while (full_string[count_bytes] != '\n' && full_string[count_bytes] != '\0')
		count_bytes++;
	count_remainder = (gnl_strlen(full_string) + 1);
	if ((count_remainder - count_bytes) > 0)
		buffer_remainder = gnl_calloc(count_remainder + 1, sizeof(char));
	if (!buffer_remainder)
		return (NULL);
	fill_remainder = 0;
	while (full_string[count_bytes] != '\0')
		buffer_remainder[fill_remainder++] = full_string[++count_bytes];
	free(full_string);
	return (buffer_remainder);
}

int	check_read(int read_bytes, char ***full_string)
{
	if (read_bytes == -1)
	{
		if (**full_string)
		{
			free(**full_string);
			**full_string = NULL;
		}
		return (0);
	}
	if (read_bytes == 0)
		return (0);
	return (1);
}
