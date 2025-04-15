/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:18 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:19 by mzary            ###   ########.fr       */
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

t_status	lexer_lex(t_lexer *lexer)
{
	t_status	status;

	status = lex_lex(lexer, lexer->splited_cmdline);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
}
