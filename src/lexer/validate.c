/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:13 by mzary             #+#    #+#             */
/*   Updated: 2025/05/07 13:29:10 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/lexer.h"

static bool	validate_paren(t_token *token)
{
	uint32_t	count;

	if (token)
	{
		count = 0;
		while (token)
		{
			if (token->ttype == TTOKEN_PARENTHESE_OPEN)
				count += minishell_strlen(token->tvalue);
			else if (token->ttype == TTOKEN_PARENTHESE_CLOSE)
				count -= minishell_strlen(token->tvalue);
			token = token->next_token;
		}
		if (count == 0)
			return (true);
		return (false);
	}
	return (false);
}

bool	validate_pipe(t_token *token)
{
	if (token->ttype == TTOKEN_PIPE)
	{
		if (token->next_token && token->next_token->ttype != TTOKEN_COMMAND)
			return (false);
	}
	return (true);
}

t_status	lexer_validate(t_token *token)
{
	bool	tflag;

	tflag = true;
	while (token)
	{
		if (!validate_pipe(token))
			return (STATUS_SYNTAXERR);
		if (token->ttype == TTOKEN_HEREDOC)
		{
			if (token->next_token->ttype != TTOKEN_HEREDOC_KEYWORD)
				return (STATUS_SYNTAXERR);
		}
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
