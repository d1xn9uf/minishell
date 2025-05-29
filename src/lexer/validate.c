/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:13 by mzary             #+#    #+#             */
/*   Updated: 2025/05/15 18:10:07 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/lexer.h"

static bool	validate_close_paren(t_token *token)
{
	if (!token)
		return (true);
	if (token->ttype == TTOKEN_PARENTHESE_CLOSE)
		return (true);
	if (minishell_iscmdop(token))
		return (true);
	return (false);
}

static bool	validate_paren(t_token *token)
{
	uint32_t	count;

	if (token)
	{
		count = 0;
		while (token)
		{
			if (token->ttype == TTOKEN_PARENTHESE_OPEN)
			{
				if (token->next_token
					&& token->next_token->ttype == TTOKEN_PARENTHESE_CLOSE)
					return (false);
				count += minishell_strlen(token->tvalue);
			}
			else if (token->ttype == TTOKEN_PARENTHESE_CLOSE)
			{
				if (!validate_close_paren(token->next_token))
					return (false);
				count -= minishell_strlen(token->tvalue);
			}
			token = token->next_token;
		}
		return (!count);
	}
	return (false);
}

bool	validate_pipe_and_or(t_token *token)
{
	if (token->ttype == TTOKEN_PIPE)
	{
		if (token->next_token
			&& (token->next_token->ttype != TTOKEN_COMMAND
				&& !minishell_isred(token->next_token))
			&& token->next_token->ttype != TTOKEN_PARENTHESE_OPEN)
			return (false);
	}
	if (token->ttype == TTOKEN_AND_OP || token->ttype == TTOKEN_OR_OP)
	{
		if (!token->next_token)
			return (false);
	}
	return (true);
}

bool	validate_red(t_token *token, t_status *status, int32_t *hdoc_count)
{
	if (minishell_isred(token))
	{
		if (!token->next_token
			|| (token->next_token->ttype != TTOKEN_FILE
				&& token->next_token->ttype != TTOKEN_HEREDOC_KEYWORD))
			return (false);
		if (token->ttype == TTOKEN_HEREDOC)
		{
			if (*hdoc_count == 16)
			{
				minishell_stderr("minishell: maximum here-document count exceeded\n",
					NULL, NULL);
				*status = STATUS_TOMANYHDOC;
				return (false);
			}
			(*hdoc_count)++;
		}
	}
	return (true);
}

t_status	lexer_validate(t_token *token, t_status *status, int32_t *hdoc_count)
{
	bool	tflag;

	tflag = true;
	while (token)
	{
		if (!validate_red(token, status, hdoc_count))
			return (STATUS_SYNTAXERR);
		if (!validate_pipe_and_or(token))
			return (STATUS_SYNTAXERR);
		if (token->ttype == TTOKEN_HEREDOC && token->next_token
			&& token->next_token->ttype != TTOKEN_HEREDOC_KEYWORD)
			return (STATUS_SYNTAXERR);
		if (token->ttype == TTOKEN_UNKOWN)
			return (STATUS_SYNTAXERR);
		if (tflag && token->ttype == TTOKEN_PARENTHESE_OPEN)
		{
			if (!validate_paren(token))
				return (STATUS_SYNTAXERR);
			tflag = false;
		}
		if (tflag && token->ttype == TTOKEN_PARENTHESE_CLOSE)
			return (STATUS_SYNTAXERR);
		token = token->next_token;
	}
	return (STATUS_SUCCESS);
}
