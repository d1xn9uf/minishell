/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:53 by mzary             #+#    #+#             */
/*   Updated: 2025/05/28 11:03:01 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	update(t_token *token, t_env *env, t_args args);
static t_status	update_token(t_token *token, t_env *env, t_args args);
static bool		check_flag(t_token *token, bool	prev_flag);
static bool		quoted_empty(char *value);

t_status	minishell_translate(t_token *root, t_env *env, char *str_exitcode)
{
	t_status	status;
	t_args		args;

	args.exit = str_exitcode;
	args.flag = check_flag(root, true);
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
	if (token->tvalue && *token->tvalue && !token->is_interpreted
		&& !quoted_empty(token->tvalue))
	{
		s = update_token(token, env, args);
		if (s)
			return (s);
	}
	else if (token->tvalue && quoted_empty(token->tvalue)
		&& minishell_remove(token))
		return (STATUS_MALLOCERR);
	args.flag = check_flag(token, args.flag);
	if (token->right)
		token->right->ambig.red_flag = token->ambig.red_flag;
	if (token->left)
		token->left->ambig.red_flag = token->ambig.red_flag;
	s = update(token->right, env, args);
	if (s)
		return (s);
	s = update(token->left, env, args);
	return (s);
}

static t_status	update_token(t_token *token, t_env *env, t_args args)
{
	t_status	s;

	if (!quoted_empty(token->tvalue))
	{
		s = minishell_interpret(token, env, args);
		if (token->ambig.red_flag && !token->ambig.is_ambiguous)
			minishell_free((void **)&token->ambig.saver);
	}
	else
		s = minishell_remove(token);
	if (s)
			return (s);
	if (token->ttype == TTOKEN_COMMAND
		&& !minishell_isbuiltin(token->tvalue))
		s = update_command(token, env);
	return (s);
}

static bool	check_flag(t_token *token, bool prev_flag)
{
	if (token->ttype == TTOKEN_COMMAND && token->tvalue
		&& minishell_strequal(token->tvalue, EXPORT))
		return (false);
	return (prev_flag);
}

static bool	quoted_empty(char *value)
{
	return (minishell_strequal(value, "\"\"")
		|| minishell_strequal(value, "''"));
}
