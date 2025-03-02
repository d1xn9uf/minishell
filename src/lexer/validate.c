#include "../../inc/lexer.h"

static bool	validate_paren(t_token *token)
{
	uint32_t	count;

	if (token)
	{	
		count = 0;
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
