#include "../../inc/builtins.h"

// do not forget the shell level variable

static t_status	extract_pair(t_env *pair, char *s);
static t_status	modify_node(t_env *node, char *value);
static t_status	add_node(t_env *l_env, char *key, char *value);

t_status	minishell_export(char **argv, t_env *l_env) // asterisk expands a jemmi
{
	int			i;
	t_env		pair;
	t_status	status;

	if (argv && l_env)
	{
		if (!argv[1]) // fix behavior
			return (default_export(l_env)); // TODO
		i = 1;
		while (argv[i])
		{
			if (minishell_strchr(argv[i], '='))
			{
				if ((status = extract_pair(&pair, argv[i])))
					return (status);
				if ((status = export(pair.key, pair.value, l_env)))
					return (status);
			}
			i += 1;
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
		return (free(pair->value), STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

t_status	export(char *key, char *value, t_env *l_env)
{
	t_env	*node;

	if (!key || !value) // might occure when strdup is called in cd in update_wd
		return (minishell_free((void **)&key), minishell_free((void **)&value), STATUS_MALLOCERR);
	node = l_env;
	while (node)
	{
		if (minishell_strequal(key, node->key))
			return (free(key), modify_node(node, value));
		node = node->next_key;
	}
	return (add_node(l_env, key, value));
}

static t_status	modify_node(t_env *node, char *value)
{
	free(node->value);
	node->value = value;
	return (STATUS_SUCCESS);
}

static t_status	add_node(t_env *l_env, char *key, char *value)
{
	t_env	*node;

	node = l_env;
	while (node->next_key)
		node = node->next_key;
	node->next_key = (t_env *)malloc(sizeof(t_env));
	if (!node->next_key)
		return (free(key), free(value), STATUS_MALLOCERR);
	node = node->next_key;
	node->key = key;
	node->value = value;
	node->next_key = NULL;
	return (STATUS_SUCCESS);
}
