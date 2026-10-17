/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*gnl_strjoin(char const *string_line, char *buffer)
{
	char	*full_string;
	size_t	index_str;
	size_t	index_full_str;

	if (!buffer)
		return (NULL);
	full_string = gnl_calloc((gnl_strlen(string_line) + gnl_strlen(buffer) + 1),
			sizeof(char));
	if (!full_string)
		return (NULL);
	index_str = 0;
	if (string_line)
		index_full_str = gnl_strlen(string_line);
	else
		index_full_str = 0;
	while (string_line && string_line[index_str])
	{
		full_string[index_str] = string_line[index_str];
		index_str++;
	}
	index_str = 0;
	while (buffer && buffer[index_str])
		full_string[index_full_str++] = buffer[index_str++];
	full_string[index_full_str] = '\0';
	return (full_string);
}

char	*gnl_strchr(const char *string, int c)
{
	int	index;

	index = 0;
	if (!string)
		return (NULL);
	while ((unsigned char)string[index])
	{
		if ((unsigned char)string[index] == (unsigned char)c)
			return ((char *)&string[index]);
		index++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)&string[index]);
	return (NULL);
}

size_t	gnl_strlen(const char *string)
{
	size_t	length;

	if (!string)
		return (0);
	length = 0;
	while (string[length] != '\0')
		length++;
	return (length);
}

void	*gnl_calloc(size_t nmemb, size_t size)
{
	size_t	memory_size;
	void	*allocated_memory;

	memory_size = nmemb * size;
	if (nmemb == 0 || size == 0)
		memory_size = 1;
	if (((nmemb != 0) && (size != 0)) && (memory_size / size) != nmemb)
		return (NULL);
	allocated_memory = malloc(memory_size);
	if (!allocated_memory)
		return (NULL);
	gnl_bzero(allocated_memory, memory_size);
	return (allocated_memory);
}

void	gnl_bzero(void *s, size_t n)
{
	unsigned int	index;
	unsigned char	*casting_void;

	index = 0;
	casting_void = (unsigned char *)s;
	while (index < n)
	{
		casting_void[index] = 0;
		index++;
	}
}
