/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:55 by mzary             #+#    #+#             */
/*   Updated: 2025/05/26 12:58:38 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static bool	is_prefix(char *fix, char **s);
static bool	is_suffix(char *fix, char **s);
static bool	is_midfix(char *fix, char **s);

bool	minishell_matcher(t_fixe *fixe, char *s)
{
	uint32_t	c;

	if (s[0] == '.' && (!fixe->count
			|| !(fixe->fixes[0][0] == '.' && !fixe->flags[0].before)))
		return (false);
	if (!fixe->count)
		return (true);
	if (!fixe->flags[0].before && !fixe->flags[0].after)
		return (false);
	c = 0;
	while (c < fixe->count)
	{
		if (!fixe->flags[c].before && !is_prefix(fixe->fixes[c], &s))
			return (false);
		if (!fixe->flags[c].after && !is_suffix(fixe->fixes[c], &s))
			return (false);
		if (fixe->flags[c].before && fixe->flags[c].after
			&& !is_midfix(fixe->fixes[c], &s))
			return (false);
		c += 1;
	}
	return (true);
}

static bool	is_prefix(char *fix, char **s)
{
	uint32_t	i;

	i = 0;
	while ((*s)[i] && fix[i] && (*s)[i] == fix[i])
		i += 1;
	if (!fix[i])
		*s += minishell_strlen(fix);
	return (!fix[i]);
}

static bool	is_suffix(char *fix, char **s)
{
	uint32_t	fixlen;
	uint32_t	slen;

	fixlen = minishell_strlen(fix);
	slen = minishell_strlen(*s);
	if (slen < fixlen)
		return (false);
	return (minishell_strequal(fix, *s + slen - fixlen));
}

static bool	is_midfix(char *fix, char **s)
{
	uint32_t	i;
	uint32_t	fixlen;

	i = 0;
	fixlen = minishell_strlen(fix);
	while ((*s)[i])
	{
		if (!minishell_strncmp(fix, *s + i, fixlen))
		{
			*s = *s + i + fixlen;
			return (true);
		}
		i += 1;
	}
	return (false);
}
