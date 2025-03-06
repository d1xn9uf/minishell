#include "../../inc/tools.h"

static t_status	key_value(t_env *node, char *line);

t_env		*minishell_getenv(char **env)
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
	node->key = minishell_strdup(line);
	if (!node->key)
		return (STATUS_MALLOCERR);
	node->value = minishell_strdup(line + equal_index + 1);
	if (!node->value)
	{
		free(node->key);
		return (STATUS_MALLOCERR);
	}
	return (STATUS_SUCCESS);
}

char		*minishell_getvalue(t_env *env, char *key)
{
	t_env	*node;

	key += 1; //skip the $
	if (!key) // key is $
		return (minishell_strdup("$"));
	node = env;
	while (node)
	{
		if (minishell_strequal(key, node->key))
			return (node->value);
		node = node->next_key;
	}
	return (minishell_strdup(""));
}
