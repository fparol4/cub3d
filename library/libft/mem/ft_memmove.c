/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	f_upwards(unsigned char *dst, unsigned char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
}

void	f_backwards(unsigned char *dst, unsigned char *src, size_t n)
{
	size_t	i;

	i = n;
	while (i > 0)
	{
		i--;
		dst[i] = src[i];
	}
}

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char	*dst_p;
	unsigned char	*src_p;

	if (!dst && !src)
		return (NULL);
	dst_p = (unsigned char *)dst;
	src_p = (unsigned char *)src;
	if (dst_p > src_p)
		f_backwards(dst_p, src_p, n);
	else
		f_upwards(dst_p, src_p, n);
	return (dst);
}

#if __TESTING__
// #include <string.h>
// void __test__()
// {
// 	char buff_2[] = "ABCDEF";
// 	char buff_1[] = "KLM";
// 	ft_memmove(buff_1 + 2, buff_2, 3);
// 	printf("RESULT -> %s;%s", buff_1, buff_2);
// }
// int	main (void){ __test__(); }
#endif
