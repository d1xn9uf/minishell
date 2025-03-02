#include "../../inc/tools.h"

t_env	*minishell_envlist(int32_t count)
{
	t_env	*l_env;
	t_env	*node;

	if (!count) // empty environment
		return (NULL);
	l_env = (t_env *)malloc(sizeof(t_env));
	if (!l_env)
		return (NULL);
	node = l_env;
	while (--count)
	{
		node->next_key = (t_env *)malloc(sizeof(t_env));
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
		free(l_env[i].key);
		free(l_env[i].value);
		i += 1;
	}
}

void	minishell_freelst(t_env *l_env)
{
	t_env	*next;

	while (l_env)
	{
		next = l_env->next_key;
		free(l_env);
		l_env = next;
	}
}
