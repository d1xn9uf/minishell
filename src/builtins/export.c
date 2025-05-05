/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:47 by mzary             #+#    #+#             */
/*   Updated: 2025/05/04 18:10:10 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static t_status	extract_pair(t_env *pair, char *s);
static t_status	modify_node(t_env *node, char *value);
static t_status	add_node(t_env **l_env, char *key, char *value);

t_status	minishell_export(char **argv, t_env **l_env) // check if key is valid ni**a
{
	int			i;
	t_env		pair;

	if (argv && l_env)
	{
		if (!argv[1])
			return (default_export(*l_env));
		i = 0;
		while (argv[++i])
		{
			if (minishell_strchr(argv[i], '='))
			{
				if (extract_pair(&pair, argv[i]))
					return (STATUS_MALLOCERR);
				if (export(pair.key, pair.value, l_env))
					return (STATUS_MALLOCERR);
			}
			else if (export_inv(minishell_strdup(argv[i]), l_env))
				return (STATUS_MALLOCERR);
		}
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

static t_status	extract_pair(t_env *pair, char *s)
{
	uint32_t	equal_i;

	equal_i = 0;
	while (s[equal_i] != '=')
		equal_i += 1;
	pair->value = minishell_strdup(s + equal_i + 1);
	if (!pair->value)
		return (STATUS_MALLOCERR);
	s[equal_i] = 0;
	pair->key = minishell_strdup(s);
	s[equal_i] = '=';
	if (!pair->key)
		return (minishell_free((void **)&pair->value), STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

t_status	export(char *key, char *value, t_env **l_env)
{
	t_env	*node;

	if (!key || !value)
		return (minishell_free((void **)&key),
			minishell_free((void **)&value), STATUS_MALLOCERR);
	node = *l_env;
	while (node)
	{
		if (minishell_strequal(key, node->key))
			return (minishell_free((void **)&key), modify_node(node, value));
		node = node->next_key;
	}
	return (add_node(l_env, key, value));
}

static t_status	modify_node(t_env *node, char *value)
{
	minishell_free((void **)&node->value);
	node->value = value;
	return (STATUS_SUCCESS);
}

static t_status	add_node(t_env **l_env, char *key, char *value)
{
	t_env	*node;
	t_env	*last;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (minishell_free((void **)&key),
			minishell_free((void **)&value), STATUS_MALLOCERR);
	node->key = key;
	node->value = value;
	node->next_key = NULL;
	node->chosen = false;
	node->valid = true;
	if (!*l_env)
		return (*l_env = node, STATUS_SUCCESS);
	last = *l_env;
	while (last->next_key)
		last = last->next_key;
	last->next_key = node;
	return (STATUS_SUCCESS);
}
