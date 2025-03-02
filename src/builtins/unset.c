#include "../../inc/builtins.h"

static void	unset(char *key, t_env **l_env);
static void	pop_node(t_env *node, t_env **l_env);

t_status	minishell_unset(char **argv, t_env **l_env)
{
	int	i;

	if (argv && l_env && *l_env)
	{
		i = 1;
		while (argv[i])
		{
			unset(argv[i], l_env);
			i += 1;
		}
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

static void	unset(char *key, t_env **l_env)
{
	t_env	*node;

	node = *l_env;
	while (node)
	{
		if (minishell_strequal(key, node->key))
		{
			pop_node(node, l_env);
			return ;
		}		
		node = node->next_key;
	}
}

static void	pop_node(t_env *node, t_env **l_env)
{
	t_env	*b_node;
	t_env	*a_node;

	if (node == *l_env)
	{
		*l_env = node->next_key;
		free(node->key);
		free(node->value);
		free(node);
	}
	else
	{
		a_node = node->next_key;
		b_node = *l_env;
		while (b_node->next_key != node)
			b_node = b_node->next_key;
		b_node->next_key = a_node;
		free(node->key);
		free(node->value);
		free(node);
	}	
}
