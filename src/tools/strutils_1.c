/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:31 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 20:49:53 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

int32_t	minishell_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char	*minishell_strdup(const char *s)
{
	char		*c;
	uint32_t	len;

	if (!s)
		return (NULL);
	len = minishell_strlen(s);
	c = (char *)minishell_calloc(len + 1, sizeof(char));
	if (c == NULL)
		return (NULL);
	minishell_strlcpy(c, s, len + 1);
	return (c);
}

bool	minishell_strequal(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (s1 == s2);
	if (minishell_strlen(s1) == minishell_strlen(s2))
	{
		if (!minishell_strncmp(s1, s2, minishell_strlen(s1)))
			return (true);
	}
	return (false);
}

char	*minishell_strjoin(const char *s1, const char *s2)
{
	size_t	total_len;
	char	*res;

	if (!s1)
		return (minishell_strdup((char *)s2));
	if (!s2)
		return (minishell_strdup((char *)s1));
	total_len = minishell_strlen(s1) + minishell_strlen(s2);
	res = (char *)minishell_calloc(total_len + 1, sizeof(char));
	if (res == NULL)
		return (NULL);
	minishell_strlcpy(res, s1, minishell_strlen(s1) + 1);
	minishell_strlcat(res, s2, total_len + 1);
	return (res);
}

int32_t	minishell_strcmp(const char *s1, const char *s2)
{
	uint32_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i += 1;
	return (s1[i] - s2[i]);
}
