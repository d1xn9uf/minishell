/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:18 by mzary             #+#    #+#             */
/*   Updated: 2025/05/08 20:08:20 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/lexer.h"

static t_status	lex_tflag(t_lexer *lexer, char *tvalue)
{
	t_token	*ltoken;

	ltoken = lex_last_token(lexer->token);
	if ((ltoken && ltoken->ttype == TTOKEN_OUTPUT)
		|| (ltoken && ltoken->ttype == TTOKEN_APPEND)
		|| (ltoken && ltoken->ttype == TTOKEN_INPUT))
		return (lex_add_token(lexer, tvalue, TTOKEN_FILE));
	else if (ltoken && ltoken->ttype == TTOKEN_HEREDOC)
		return (lex_add_token(lexer, tvalue, TTOKEN_HEREDOC_KEYWORD));
	else
		return (lex_add_token(lexer, tvalue, TTOKEN_COMMAND));
}

static t_status	lex_lex(t_lexer *lexer, char **splited_cmd)
{
	bool		tflag;
	t_status	status;

	tflag = true;
	while (*splited_cmd)
	{
		if (minishell_iscmdsep(**splited_cmd))
		{
			status = lex_add_token(lexer, *splited_cmd,
					lex_get_token_type(*splited_cmd));
			tflag = true;
		}
		else if (tflag)
		{
			status = lex_tflag(lexer, *splited_cmd);
			tflag = false;
		}
		else
			status = lex_add_token(lexer, *splited_cmd,
					lex_get_token_type(*splited_cmd));
		if (status)
			return (status);
		splited_cmd++;
	}
	return (STATUS_SUCCESS);
}

static void	reposition_token(t_lexer *lexer, t_token *token,
				t_token **token_arg)
{
	t_token	*cmd_token;

	cmd_token = NULL;
	if (token->prev_token)
	{
		cmd_token = token->prev_token;
		if (token->next_token)
			*token_arg = token->next_token->next_token;
	}
	else
	{
		if (token->next_token)
		{
			cmd_token = token->next_token->next_token;
			if (token->next_token->next_token)
				*token_arg = token->next_token->next_token->next_token;
		}
		if (cmd_token)
		{
			move_token(&lexer->token, cmd_token->tid, token->tid);
			cmd_token->ttype = TTOKEN_COMMAND;
		}
	}
}

static void	lex_reposition_red(t_lexer *lexer)
{
	t_token	*token;
	t_token	*token_arg;
	t_token	*arg_tmp;

	token = lexer->token;
	token_arg = NULL;
	while (token)
	{
		if (minishell_isred(token))
		{
			reposition_token(lexer, token, &token_arg);
			if (token_arg)
			{
				while (token_arg && token_arg->ttype == TTOKEN_ARGUMENT)
				{
					arg_tmp = token_arg->next_token;
					move_token(&lexer->token, token_arg->tid, token->tid);
					token_arg = arg_tmp;
				}
			}
		}
		token = token->next_token;
	}
}

t_status	lexer_lex(t_lexer *lexer)
{
	t_status	status;

	status = lex_lex(lexer, lexer->splited_cmdline);
	if (status)
		return (status);
	lex_reposition_red(lexer);
	return (STATUS_SUCCESS);
}
