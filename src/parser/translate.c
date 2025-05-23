/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:53 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 16:22:02 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	update(t_token *token, t_env *env, t_args args);
static bool		check_flag(t_token *token);

t_status	minishell_translate(t_token *root, t_env *env, char *str_exitcode)
{
	t_status	status;
	t_args		args;

	args.exit = str_exitcode;
	args.flag = check_flag(root);
	status = update(root, env, args);
	minishell_free((void **)&str_exitcode);
	if (status)
		return (status);
	fix_tree(root);
	clean_tree(root);
	return (STATUS_SUCCESS);
}

static t_status	update(t_token *token, t_env *env, t_args args)
{
	t_status	s;

	if (!token)
		return (STATUS_SUCCESS);
	if (token->tvalue && *token->tvalue && !token->is_interpreted)
	{
		s = minishell_interpret(token, env, args);
		if (s)
			return (s);
		if (token->ttype == TTOKEN_COMMAND
			&& !minishell_isbuiltin(token->tvalue))
		{
			s = update_command(token, env);
			if (s)
				return (s);
		}
		token->is_interpreted = true;
	}
	args.flag = check_flag(token);
	s = update(token->right, env, args);
	if (s)
		return (s);
	return (STATUS_SUCCESS);
}

static bool	check_flag(t_token *token)
{
	if (token->ttype == TTOKEN_COMMAND && token->tvalue
		&& minishell_strequal(token->tvalue, EXPORT))
		return (false);
	return (true);
}
