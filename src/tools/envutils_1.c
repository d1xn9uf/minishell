/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envutils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:49 by mzary             #+#    #+#             */
/*   Updated: 2025/05/04 18:25:45 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

static t_status	key_value(t_env *node, char *line);
static char		*quoted_value(char *value);

t_env	*minishell_getenv(char **env)
{
	int32_t		count;
	t_env		*l_env;
	t_env		*node;
	int32_t		index;

	count = minishell_envsize(env);
	l_env = minishell_envlist(count);
	if (!l_env)
		return (NULL);
	node = l_env;
	index = 0;
	while (index < count)
	{
		if (key_value(node, env[index]))
		{
			minishell_freekv(l_env, index);
			minishell_freelst(l_env);
			return (NULL);
		}
		node->chosen = false;
		node->valid = true;
		node = node->next_key;
		index += 1;
	}
	return (l_env);
}

static t_status	key_value(t_env *node, char *line)
{
	int32_t	equal_index;

	equal_index = 0;
	while (line[equal_index] != '=')
		equal_index += 1;
	line[equal_index] = 0;
	node->key = minishell_strdup(line);
	line[equal_index] = '=';
	if (!node->key)
		return (STATUS_MALLOCERR);
	node->value = minishell_strdup(line + equal_index + 1);
	if (!node->value)
	{
		minishell_free((void **)&node->key);
		return (STATUS_MALLOCERR);
	}
	return (STATUS_SUCCESS);
}

char	*minishell_getvalue(t_env *env, char *key)
{
	t_env	*node;

	if (key)
	{
		if (*key != CHAR_DOLLAR_SIGN)
			return (minishell_strdup(""));
		key += 1;
		if (!*key)
			return (minishell_strdup("$"));
		if ('0' <= *key && *key <= '9')
			return (minishell_strdup(key + 1));
		node = env;
		while (node)
		{
			if (minishell_strequal(key, node->key) && node->valid)
				return (quoted_value(minishell_strdup(node->value)));
			node = node->next_key;
		}
		return (minishell_strdup(""));
	}
	return (NULL);
}

static char	*quoted_value(char *value)
{
	char	*qvalue;

	if (!value)
		return (NULL);
	qvalue = minishell_strjoin("\"", value);
	minishell_free((void **)&value);
	if (!qvalue)
		return (NULL);
	value = qvalue;
	qvalue = minishell_strjoin(value, "\"");
	minishell_free((void **)&value);
	return (qvalue);
}

void	minishell_envfree(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next_key;
		minishell_free((void **)&env->key);
		minishell_free((void **)&env->value);
		minishell_free((void **)&env);
		env = next;
	}
}
