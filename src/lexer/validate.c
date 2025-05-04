/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:13 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:13 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/lexer.h"

static bool	validate_paren(t_token *token)
{
	uint32_t	count;

	if (token)
	{
		count = 1;
		while (token)
		{
			if (token->ttype == TTOKEN_PARENTHESE_CLOSE && !count)
				return (true);
			if (token->ttype == TTOKEN_PARENTHESE_OPEN)
				count++;
			else if (token->ttype == TTOKEN_PARENTHESE_CLOSE)
				count--;
			token = token->next_token;
		}
		return (false);
	}
	return (false);
}

t_status	lexer_validate(t_token *token)
{
	while (token)
	{
		if (token->ttype == TTOKEN_UNKOWN)
			return (STATUS_SYNTAXERR);
		if (token->ttype == TTOKEN_PARENTHESE_OPEN)
		{
			if (!validate_paren(token->next_token))
				return (STATUS_SYNTAXERR);
		}
		token = token->next_token;
	}
	return (STATUS_SUCCESS);
}
