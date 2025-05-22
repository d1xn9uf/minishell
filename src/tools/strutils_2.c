/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:30 by mzary             #+#    #+#             */
/*   Updated: 2025/05/14 15:51:15 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

static int	s_count(t_split *split);
static int	ft_split(t_split *split);

char	**minishell_split(char *s, char c, bool *flags)
{
	t_split	split;

	split.s = s;
	if (!split.s)
		return (NULL);
	split.flags = flags;
	split.c = c;
	split.count = s_count(&split);
	split.split = (char **)minishell_calloc(split.count + 1, sizeof(char *));
	if (split.split == NULL)
		return (NULL);
	if (!ft_split(&split))
		return (NULL);
	return (split.split);
}

static int	s_count(t_split *split)
{
	int			i;

	i = 0;
	split->count = 0;
	while (split->s[i])
	{
		if (split->s[i] == split->c && (!split->flags || split->flags[i]))
			i++;
		else
		{
			split->count++;
			while ((split->s[i] && split->s[i] != split->c)
				|| (split->s[i] && split->flags && !split->flags[i]))
				i++;
		}
	}
	return (split->count);
}

static int	ft_split(t_split *split)
{
	int	i;
	int	st;
	int	split_i;

	i = 0;
	split_i = -1;
	while (++split_i < split->count)
	{
		while (split->s[i] && split->s[i] == split->c
			&& (!split->flags || split->flags[i]))
			i++;
		st = i;
		while ((split->s[i] && split->s[i] != split->c)
			|| (split->s[i] && split->flags && !split->flags[i]))
			i++;
		split->split[split_i] = (char *)minishell_calloc(i - st + 1,
				sizeof(char));
		if (split->split[split_i] == NULL)
			return (minishell_free_arr(split->split), 0);
		minishell_strlcpy(split->split[split_i], split->s + st, i - st + 1);
	}
	split->split[split->count] = NULL;
	return (1);
}

size_t	minishell_strlcat(char *dst, const char *src, size_t size)
{
	uint32_t	dst_len;
	uint32_t	src_len;

	if (!dst && !size)
		return (minishell_strlen(src));
	dst_len = minishell_strlen((const char *)dst);
	src_len = minishell_strlen((const char *)src);
	if (size >= dst_len + 1)
	{
		minishell_strlcpy(dst + dst_len, src, size - dst_len);
		return (dst_len + src_len);
	}
	else
		return (size + src_len);
}

bool	is_separator(char c)
{
	if (c == '_')
		return (false);
	if ('a' <= c && c <= 'z')
		return (false);
	if ('A' <= c && c <= 'Z')
		return (false);
	if ('0' <= c && c <= '9')
		return (false);
	return (true);
}
