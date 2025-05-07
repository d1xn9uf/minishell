/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:47 by mzary             #+#    #+#             */
/*   Updated: 2025/05/07 13:40:04 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static t_status	extract_pair(t_env *pair, char *s);
static t_status	modify_node(t_env *node, char *value, bool append);
static t_status	add_node(t_env **l_env, char *key, char *value);

t_status	minishell_export(char **argv, t_env **l_env)
{
	t_env		pair;
	t_status	status;

	if (!argv || !l_env)
		return (STATUS_FAILURE);
	if (!argv[1])
		return (default_export(*l_env));
	while (*(++argv))
	{
		if (minishell_strchr(*argv, '='))
		{
			if (extract_pair(&pair, *argv))
				return (STATUS_MALLOCERR);
			status = export(pair.key, pair.value, l_env);
			if (status)
				return (status);
		}
		else
		{
			status = export_inv(minishell_strdup(*argv), l_env);
			if (status)
				return (status);
		}
	}
	return (STATUS_SUCCESS);
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
	bool	append;

	if (!key || !value)
		return (minishell_free((void **)&key),
			minishell_free((void **)&value), STATUS_MALLOCERR);
	append = false;
	if (key[minishell_strlen(key) - 1] == '+' && setbool(&append, true))
	{
		key[minishell_strlen(key) - 1] = 0;
		if (!minishell_validkey(key) && minishell_free((void **)&key))
			return (printf("minishell_export: %s: not a valid id\n", key),
				minishell_free((void **)&value), STATUS_FAILURE);
	}
	else if (!minishell_validkey(key) && minishell_free((void **)&key))
		return (printf("minishell_export: %s: not a valid id\n", key),
			minishell_free((void **)&value), STATUS_FAILURE);
	node = *l_env;
	while (node)
	{
		if (minishell_strequal(key, node->key) && minishell_free((void **)&key))
			return (modify_node(node, value, append));
		node = node->next_key;
	}
	return (add_node(l_env, key, value));
}

static t_status	modify_node(t_env *node, char *value, bool append)
{
	char	*old_value;

	minishell_free((void **)&node->value);
	if (append && node->value)
	{
		old_value = node->value;
		node->value = minishell_strjoin(old_value, value);
		minishell_free((void **)&value);
		if (!node->value)
			return (node->value = old_value, STATUS_MALLOCERR);
		minishell_free((void **)&old_value);
	}
	else
		node->value = value;
	node->valid = true;
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
