/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:51 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:51 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static void	env(t_env *l_env);

t_status	minishell_env(char **argv, t_env *l_env)
{
	if (argv && l_env)
	{
		if (argv[1])
		{
			minishell_stderr("minishell_env: ",
				argv[1], ": No such file or directory\n");
			return (STATUS_FAILURE);
		}
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
		if (node->valid)
			printf("%s=%s\n", node->key, node->value);
		node = node->next_key;
	}
}
