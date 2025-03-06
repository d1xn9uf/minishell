#include "../../inc/builtins.h"

static t_status	export(char *arg, t_env *l_env);
static t_status	modify_node(t_env *node, char *value);
static t_status	add_node(t_env *l_env, char *key, char *value);

t_status	minishell_export(char **argv, t_env *l_env) // asterisk expands a jemmi
{
	int	i;

	if (argv && l_env)
	{
		if (!argv[1])
			return (STATUS_FAILURE);
		i = 1;
		while (argv[i])
		{
			if (minishell_strchr(argv[i], '=') && export(argv[i], l_env))
				return (STATUS_MALLOCERR);
			i += 1;
		}
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

static t_status	export(char *arg, t_env *l_env)
{
	int		equal_i;
	char	*key;
	char	*value;
	t_env	*node;

	equal_i = 0;
	while (arg[equal_i] != '=')
		equal_i += 1;
	value = minishell_strdup(arg + equal_i + 1);
	if (!value)
		return (STATUS_MALLOCERR);
	arg[equal_i] = 0;
	key = minishell_strdup(arg);
	if (!key)
		return (free(value), STATUS_MALLOCERR);
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
