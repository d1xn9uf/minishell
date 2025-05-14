/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reposition.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:13:34 by mzary             #+#    #+#             */
/*   Updated: 2025/05/14 16:14:43 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

static void	reposition_cmd(t_lexer *lexer, t_token *token, t_token **token_arg)
{
	t_token	*cmd_token;
	t_token	*tmp;

	if (token->next_token)
	{
		tmp = token;
		while (tmp->next_token)
		{
			if (tmp->next_token->ttype == TTOKEN_ARGUMENT)
			{
				cmd_token = tmp->next_token;
				break ;
			}
			tmp = tmp->next_token;
		}
	}
	if (cmd_token)
	{
		*token_arg = cmd_token->next_token;
		move_token(&lexer->token, cmd_token->tid, token->tid);
		cmd_token->ttype = TTOKEN_COMMAND;
	}
}

static void	reposition_token(t_lexer *lexer, t_token *token,
	t_token **token_arg)
{
	if (token->prev_token
		&& token->prev_token->ttype == TTOKEN_COMMAND)
	{
		if (token->next_token)
			*token_arg = token->next_token->next_token;
	}
	else
		reposition_cmd(lexer, token, token_arg);
}

void	lex_reposition_red(t_lexer *lexer)
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
