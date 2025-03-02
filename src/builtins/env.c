#include "../../inc/builtins.h"

static void	env(t_env *l_env);

t_status	minishell_env(char **argv, t_env *l_env)
{
	if (argv && l_env)
	{
		if (argv[1])
			return (STATUS_FAILURE);
		env(l_env);
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

static void	env(t_env *l_env)
{
	t_env	*node;

	node = l_env;
	while (node)
	{
		printf("%s=%s\n", node->key, node->value);
		node = node->next_key;
	}
}
