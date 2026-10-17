/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:19 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:19 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdio.h>

static int	count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s != c && *s)
				s++;
		}
		else
			++s;
	}
	return (count);
}

static int	clean_all(char **split, int idx_word)
{
	while (idx_word >= 0)
	{
		free(split[idx_word]);
		idx_word--;
	}
	free(split);
	return (0);
}

static int	crop_words(char **split, char const *s, char c, int word_count)
{
	int	w_len;
	int	word_idx;

	word_idx = 0;
	while (word_idx < word_count)
	{
		while (*s)
		{
			w_len = 0;
			if (s[w_len] != c)
			{
				while (s[w_len] != c && s[w_len])
					w_len++;
				split[word_idx++] = ft_substr(s, 0, w_len);
				s += w_len;
				if (!split[word_idx - 1])
					return (clean_all(split, word_idx));
			}
			else
				s++;
		}
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		word_count;
	char	**split;

	word_count = count_words(s, c);
	split = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!split)
		return (NULL);
	if (!crop_words(split, s, c, word_count))
		return (NULL);
	split[word_count] = 0;
	return (split);
}
