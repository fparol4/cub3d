/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:07 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:07 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	_split_count_words(char const *s, char c)
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

static int	_split_cleanup(char **split, int count)
{
	while (count > 0)
		free(split[--count]);
	free(split);
	return (0);
}

static int	_split_crop_words(char **split, char const *s, char c, int count)
{
	int	w_len;
	int	word_idx;

	word_idx = 0;
	while (word_idx < count)
	{
		while (*s == c)
			s++;
		w_len = 0;
		while (s[w_len] && s[w_len] != c)
			w_len++;
		split[word_idx] = ft_substr(s, 0, w_len);
		if (!split[word_idx])
			return (_split_cleanup(split, word_idx));
		word_idx++;
		s += w_len;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		word_count;
	char	**split;

	word_count = _split_count_words(s, c);
	split = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!split)
		return (NULL);
	if (!_split_crop_words(split, s, c, word_count))
		return (NULL);
	split[word_count] = 0;
	return (split);
}
