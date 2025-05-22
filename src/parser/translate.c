/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:53 by mzary             #+#    #+#             */
/*   Updated: 2025/05/22 14:21:44 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	update(t_token *token, t_env *env, t_args args);
static bool		check_flag(t_token *token);
static void		fix_tree(t_token *token);
static void		clean_tree(t_token *token);

t_status	minishell_translate(t_token *root, t_env *env, char *str_exitcode)
{
	t_status	status;
	t_args		args;

	args.exit = str_exitcode;
	args.flag = check_flag(root);
	args.step = 0;
	status = update(root, env, args);
	if (status)
		return (minishell_free((void **)&str_exitcode), status);
	args.step = 1;
	status = update(root, env, args);
	if (status)
		return (minishell_free((void **)&str_exitcode), status);
	fix_tree(root);
	clean_tree(root);
	minishell_free((void **)&str_exitcode);
	return (STATUS_SUCCESS);
}

static t_status	update(t_token *token, t_env *env, t_args args)
{
	t_status	s;

	if (!token)
		return (STATUS_SUCCESS);
	if (args.step == 0
		&& (minishell_fillflags(token) || minishell_remove(token)))
		return (STATUS_MALLOCERR);
	/* debug */
	// printf("dollar sign flags: ");
	// for (uint32_t i = 0; i < token->d.n; i++)
	// 	printf("[%d] ", token->d.flags[i]);
	// printf("\n");
	// printf("asterisk flags: ");
	// for (uint32_t i = 0; i < token->a.n; i++)
	// 	printf("[%d] ", token->a.flags[i]);
	// printf("\n");
	// printf("token after quotes removal [%s]\n", token->tvalue);
	/* debug */
	s = minishell_interpret(token, env, args);
	if (s)
		return (s);
	if (args.step == 1 && token->ttype == TTOKEN_COMMAND
		&& !minishell_isbuiltin(token->tvalue))
	{
		s = update_command(token, env);
		if (s)
			return (s);
	}
	args.flag = check_flag(token);
	s = update(token->right, env, args);
	if (s)
		return (s);
	return (STATUS_SUCCESS);
}

static bool	check_flag(t_token *token)
{
	if (token->ttype == TTOKEN_COMMAND
		&& minishell_strequal(token->tvalue, EXPORT))
		return (false);
	return (true);
}

static void	fix_tree(t_token *token)
{
	t_token	*rright;

	if (token)
	{
		if (token->right && !token->right->tvalue)
		{
			rright = token->right->right;
			minishell_free((void **)&token->right);
			token->right = rright;
		}
		fix_tree(token->right);
	}
}

static void	clean_tree(t_token *token)
{
	if (token)
	{
		if ((token->left && token->left->ttype == TTOKEN_PARENTHESE_CLOSE)
			|| (token->left && token->left->ttype == TTOKEN_PARENTHESE_OPEN))
		{
			minishell_free((void **)&token->left->tvalue);
			minishell_free((void **)&token->left);
			token->left = NULL;
		}
		if ((token->right && token->right->ttype == TTOKEN_PARENTHESE_CLOSE)
			|| (token->right && token->right->ttype == TTOKEN_PARENTHESE_OPEN))
		{
			minishell_free((void **)&token->right->tvalue);
			minishell_free((void **)&token->right);
			token->right = NULL;
		}
		clean_tree(token->left);
		clean_tree(token->right);
	}
}
