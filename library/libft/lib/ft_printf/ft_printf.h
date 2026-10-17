/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:06 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:06 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../../libft.h"

int	ft_handle_int(int value);
int	ft_handle_unsigned(unsigned int value);
int	ft_handle_hex(unsigned int v, int upper);
int	ft_handle_ptr(void *address);

#endif
