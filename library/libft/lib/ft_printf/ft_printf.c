/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"
#include <limits.h>
#include <stdarg.h>

static int	_fn_handle(char symbol, va_list *params)
{
	char	*string;

	if (symbol == 'c')
		return ((int)ft_putchar_fd((char)va_arg(*params, int), 1));
	else if (symbol == '%')
		return ((int)ft_putchar_fd('%', 1));
	else if (symbol == 's')
	{
		string = va_arg(*params, char *);
		if (!string)
			string = "(null)";
		return ((int)ft_putstr_fd(string, 1));
	}
	else if (symbol == 'd' || symbol == 'i')
		return (ft_handle_int(va_arg(*params, int)));
	else if (symbol == 'u')
		return (ft_handle_unsigned(va_arg(*params, unsigned int)));
	else if (symbol == 'p')
		return (ft_handle_ptr(va_arg(*params, void *)));
	else if (symbol == 'x')
		return (ft_handle_hex(va_arg(*params, unsigned int), 0));
	else if (symbol == 'X')
		return (ft_handle_hex(va_arg(*params, unsigned int), 1));
	return (0);
}

static int	_printf_write_one(const char *format, int *index,
					va_list *params)
{
	int	written;

	if (format[*index] == '%' && format[*index + 1]
		&& ft_strchr("cspdiuxX%", format[*index + 1]))
	{
		written = _fn_handle(format[*index + 1], params);
		*index += 2;
		return (written);
	}
	return ((int)ft_putchar_fd(format[(*index)++], 1));
}

int	ft_printf(const char *format, ...)
{
	int		i;
	int		printed;
	int		written;
	va_list	params;

	i = 0;
	printed = 0;
	if (!format)
		return (-1);
	va_start(params, format);
	while (format[i] != '\0')
	{
		written = _printf_write_one(format, &i, &params);
		if (written < 0 || printed > INT_MAX - written)
			return (va_end(params), -1);
		printed += written;
	}
	va_end(params);
	return (printed);
}
