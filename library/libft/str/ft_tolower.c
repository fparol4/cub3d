/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:19 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:19 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_tolower(int c)
{
	int	k;

	k = ('A' - 'a');
	if (c >= 'A' && c <= 'Z')
		return (c - k);
	return (c);
}

#if __TESTING__
// void __test__()
// {
// 	char buff[] = "AaBb";
// 	for (int i = 0; i < 4; i++)
// 		printf("R: %c \n", ft_tolower(buff[i]));
// }
// int	main (void){ __test__(); }
#endif
