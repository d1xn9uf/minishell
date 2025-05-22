/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:53 by mzary             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/22 14:13:15 by mzary            ###   ########.fr       */
=======
/*   Updated: 2025/05/13 21:21:58 by mzary            ###   ########.fr       */
>>>>>>> parent of 155e0fb (default message)
/*                                                                            */
/* ************************************************************************** */

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
		if (smallest->valid)
			printf("declare -x %s=\"%s\"\n", smallest->key, smallest->value);
		else
			printf("declare -x %s\n", smallest->key);
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

t_status	export_inv(char *arg, t_env **l_env)
{
	t_env	*node;
	t_env	*last;

	if (!arg)
		return (STATUS_MALLOCERR);
	if (!minishell_validkey(arg))
		return (minishell_stderr("minishell_export: ",
				arg, ": not a valid id\n"),
			minishell_free((void **)&arg), STATUS_FAILURE);
	node = (t_env *)minishell_calloc(1, sizeof(t_env));
	if (!node)
		return (minishell_free((void **)&arg), STATUS_MALLOCERR);
	node->key = arg;
	node->value = NULL;
	node->next_key = NULL;
	node->chosen = false;
	node->valid = false;
	if (!*l_env)
		return (*l_env = node, STATUS_SUCCESS);
	last = *l_env;
	while (last->next_key)
		last = last->next_key;
	last->next_key = node;
	return (STATUS_SUCCESS);
}

void	invalid_key(char *key, char *value, bool append)
{
	if (append)
	{
		minishell_stderr("minishell_export: ", key, "+=");
		minishell_stderr(value, ": not a valid id\n", NULL);
	}
	else
	{
		minishell_stderr("minishell_export: ", key, "=");
		minishell_stderr(value, ": not a valid id\n", NULL);
	}
}
<<<<<<< HEAD

static bool	already_in(char *arg, t_env *env)
{
	while (env)
	{
		if (minishell_strequal(arg, env->key))
			return (true);
		env = env->next_key;
	}
	return (false);
}
=======
>>>>>>> parent of 155e0fb (default message)
