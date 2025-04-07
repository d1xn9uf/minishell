#include "../../inc/lexer.h"

static	t_token_type	cmdsep_token_type(char *token_value)
{
	if (*token_value == CHAR_PIPE && *(token_value + 1) == CHAR_PIPE)
		return (TTOKEN_OR_OP);
	if (*token_value == CHAR_PIPE)
		return (TTOKEN_PIPE);
	if (*token_value == CHAR_AMPERSNAD && *(token_value + 1) == CHAR_AMPERSNAD)
		return (TTOKEN_AND_OP);
	if (token_value[0] == CHAR_GT && token_value[1] == CHAR_GT)
		return (TTOKEN_APPEND);
	if (token_value[0] == CHAR_LT && token_value[1] == CHAR_LT)
		return (TTOKEN_HEREDOC);
	if (token_value[0] == CHAR_GT)
		return (TTOKEN_OUTPUT);
	if (token_value[0] == CHAR_LT)
		return (TTOKEN_INPUT);
	if (*token_value == CHAR_PAREN_OPEN)
		return (TTOKEN_PARENTHESE_OPEN);
	if (*token_value == CHAR_PAREN_CLOSE)
		return (TTOKEN_PARENTHESE_CLOSE);
	return (TTOKEN_UNKOWN);
}

t_token_type	lex_get_token_type(char *token_value)
{
	if (minishell_iscmdsep(*token_value))
		return (cmdsep_token_type(token_value));
	else
		return (TTOKEN_ARGUMENT);
}

t_token	*lex_last_token(t_token *token)
{
	if (token)
	{
		while (token->next_token)
			token = token->next_token;
		return (token);
	}
	return (NULL);
}

t_status	lex_add_token(t_lexer *lexer, char *tvalue, t_token_type ttype)
{
	t_token			*token;
	static uint32_t	tid;

	token = (t_token *)minishell_calloc(1, sizeof(t_token));
	if (!token)
		return (STATUS_MALLOCERR);
	token->ttype = ttype;
	token->tid = tid++;
	token->tvalue = tvalue;
	token->next_token = NULL;
	if (!lexer->token)
		lexer->token = token;
	else
		lex_last_token(lexer->token)->next_token = token;
	return (STATUS_SUCCESS);
}
