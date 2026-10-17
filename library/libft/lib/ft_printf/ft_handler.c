/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include <stdint.h>

static int	_print_owned(char *str)
{
	ssize_t		written;

	if (!str)
		return (-1);
	written = ft_putstr_fd(str, 1);
	free(str);
	if (written > INT_MAX)
		return (-1);
	return ((int)written);
}

int	ft_handle_int(int value)
{
	char	*v_str;
	char	*b_dec;

	b_dec = "0123456789";
	v_str = ft_itoab(value, b_dec);
	return (_print_owned(v_str));
}

int	ft_handle_unsigned(unsigned int value)
{
	char	*v_str;
	char	*b_dec;

	b_dec = "0123456789";
	v_str = ft_itoab_uns(value, b_dec);
	return (_print_owned(v_str));
}

int	ft_handle_hex(unsigned int v, int upper)
{
	char	*b_hex;
	char	*v_str;

	b_hex = "0123456789abcdef";
	if (upper)
		b_hex = "0123456789ABCDEF";
	v_str = ft_itoab_uns(v, b_hex);
	return (_print_owned(v_str));
}

int	ft_handle_ptr(void *pointer)
{
	char		*b_hex;
	char		*v_str;
	ssize_t		written;

	b_hex = "0123456789abcdef";
	v_str = ft_itoab_uns((size_t)(uintptr_t)pointer, b_hex);
	if (!v_str)
		return (-1);
	written = ft_putstr_fd("0x", 1);
	if (written < 0)
		return (free(v_str), -1);
	if (ft_putstr_fd(v_str, 1) < 0)
		return (free(v_str), -1);
	written += (ssize_t)ft_strlen(v_str);
	free(v_str);
	return ((int)written);
}
