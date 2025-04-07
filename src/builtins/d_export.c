#include "../../inc/builtins.h"

static void	reset_chosen(t_env *env);

t_status	default_export(t_env *env)
{
	t_env	*smallest;
	t_env	*node;

	while (true)
	{
		smallest = env;
		while (smallest && smallest->chosen)
			smallest = smallest->next_key;
		if (!smallest)
			break ;
		node = smallest;
		while (node)
		{
			if (!node->chosen && minishell_strcmp(node->key, smallest->key) < 0)
				smallest = node;
			node = node->next_key;
		}
		smallest->chosen = true;
		printf("declare -x %s=%s\n", smallest->key, smallest->value);
	}
	reset_chosen(env);
	return (STATUS_SUCCESS);
}

static void	reset_chosen(t_env *env)
{
	while (env)
	{
		env->chosen = false;
		env = env->next_key;
	}
}
