/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:13 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:37 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static void	unset(char *key, t_env **l_env);
static void	pop_node(t_env *node, t_env **l_env);

t_status	minishell_unset(char **argv, t_env **l_env)
{
	int	i;

	if (argv && l_env)
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
		if (minishell_strequal(key, node->key) && node->valid)
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
		minishell_free((void **)&node->key);
		minishell_free((void **)&node->value);
		minishell_free((void **)&node);
	}
	else
	{
		a_node = node->next_key;
		b_node = *l_env;
		while (b_node->next_key != node)
			b_node = b_node->next_key;
		b_node->next_key = a_node;
		minishell_free((void **)&node->key);
		minishell_free((void **)&node->value);
		minishell_free((void **)&node);
	}
}
