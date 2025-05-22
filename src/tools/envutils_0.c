/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envutils_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:50 by mzary             #+#    #+#             */
/*   Updated: 2025/05/13 21:25:22 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

t_env	*minishell_envlist(int32_t count)
{
	t_env	*l_env;
	t_env	*node;

	if (!count)
		return (NULL);
	l_env = (t_env *)minishell_calloc(1, sizeof(t_env));
	if (!l_env)
		return (NULL);
	node = l_env;
	while (--count)
	{
		node->next_key = (t_env *)minishell_calloc(1, sizeof(t_env));
		if (!node->next_key)
		{
			minishell_freelst(l_env);
			return (NULL);
		}
		node = node->next_key;
	}
	node->next_key = NULL;
	return (l_env);
}

int32_t	minishell_envsize(char **env)
{
	int32_t	count;

	count = 0;
	while (env[count])
		count += 1;
	return (count);
}

void	minishell_freekv(t_env *l_env, int32_t index)
{
	int32_t	i;

	i = 0;
	while (i < index)
	{
		minishell_free((void **)&l_env[i].key);
		minishell_free((void **)&l_env[i].value);
		i += 1;
	}
}

void	minishell_freelst(t_env *l_env)
{
	t_env	*next;

	while (l_env)
	{
		next = l_env->next_key;
		minishell_free((void **)&l_env);
		l_env = next;
	}
}

char	*minishell_unquoted(t_env *env, char *key)
{
	char		*value;
	char		*unqoted;
	uint32_t	len;

	value = minishell_getvalue(env, key);
	if (!value)
		return (NULL);
	len = minishell_strlen(value);
	if (value[0] == CHAR_DOUBLE_QUOTE && value[len - 1] == CHAR_DOUBLE_QUOTE)
	{
		value[len - 1] = 0;
		unqoted = minishell_strdup(value + 1);
		value[len - 1] = CHAR_DOUBLE_QUOTE;
		minishell_free((void **)&value);
		return (unqoted);
	}
	return (value);
}
