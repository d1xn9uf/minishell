/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 20:51:41 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 15:50:51 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

char	*minishell_strjoin3(char *s1, char *s2, char *s3)
{
	char	*res1;
	char	*res2;

	res1 = minishell_strjoin(s1, s2);
	if (!res1)
		return (NULL);
	res2 = minishell_strjoin(res1, s3);
	minishell_free((void **)&res1);
	return (res2);
}

char	*minishell_substr(char *str, uint32_t s, uint32_t e)
{
	char	*substr;
	char	c;

	if (!str || s > e)
		return (NULL);
	if (e >= minishell_strlen(str))
		return (str);
	c = str[e];
	str[e] = 0;
	substr = minishell_strdup(str + s);
	str[e] = c;
	return (substr);
}

uint32_t	minishell_count(char *s, char c)
{
	uint32_t	count;
	uint32_t	i;

	count = 0;
	i = 0;
	while (s && s[i])
	{
		count += (s[i] == c);
		i += 1;
	}
	return (count);
}
