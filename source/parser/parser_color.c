/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_color.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static void	_parser_skip_space(const char **value)
{
	while (**value == ' ' || **value == '\t')
		(*value)++;
}

static int	_parser_parse_rgb_value(const char **value, int *component)
{
	int	number;

	_parser_skip_space(value);
	if (**value < '0' || **value > '9')
		return (0);
	number = 0;
	while (**value >= '0' && **value <= '9')
	{
		number = number * 10 + (**value - '0');
		if (number > 255)
			return (0);
		(*value)++;
	}
	_parser_skip_space(value);
	*component = number;
	return (1);
}

static int	_parser_parse_separator(const char **value)
{
	if (**value != ',')
		return (0);
	(*value)++;
	return (1);
}

static int	_parser_parse_rgb(const char *value, int *red, int *green, int *blue)
{
	if (!_parser_parse_rgb_value(&value, red) || !_parser_parse_separator(&value))
		return (0);
	if (!_parser_parse_rgb_value(&value, green) || !_parser_parse_separator(&value))
		return (0);
	if (!_parser_parse_rgb_value(&value, blue))
		return (0);
	return (*value == '\0');
}

int	parser_parse_color(const char *value, int *color)
{
	int	red;
	int	green;
	int	blue;

	if (!value || !color || !_parser_parse_rgb(value, &red, &green, &blue))
		return (0);
	*color = (red << 16) | (green << 8) | blue;
	return (1);
}
