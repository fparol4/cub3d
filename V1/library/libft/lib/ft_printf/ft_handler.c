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

int	ft_handle_int(int value)
{
	char	*v_str;
	char	*b_dec;

	b_dec = "0123456789";
	v_str = ft_itoab(value, b_dec);
	return (ft_putstr_fd(v_str, 1));
}

int	ft_handle_unsigned(unsigned int value)
{
	char	*v_str;
	char	*b_dec;

	b_dec = "0123456789";
	v_str = ft_itoab_uns(value, b_dec);
	return (ft_putstr_fd(v_str, 1));
}

int	ft_handle_hex(unsigned int v, int upper)
{
	char	*b_hex;
	char	*v_str;

	b_hex = "0123456789abcdef";
	if (upper)
		b_hex = "0123456789ABCDEF";
	v_str = ft_itoab_uns(v, b_hex);
	return (ft_putstr_fd(v_str, 1));
}

int	ft_handle_ptr(size_t address)
{
	char	*b_hex;
	char	*v_str;

	b_hex = "0123456789abcdef";
	v_str = ft_itoab_uns((size_t)address, b_hex);
	return (ft_putstr_fd("0x", 1) + ft_putstr_fd(v_str, 1));
}
