#include "../../inc/lexer.h"

static t_status	lexer_cmd(t_lexer *lexer)
{
	t_status	status;

	if (lexer)
	{
		status = lexer_cmd_spaced(lexer);
		if (status)
			return (status);
		status = lexer_cmd_split(lexer);
		if (status)
			return (status);
		return (STATUS_SUCCESS);
	}
	return (STATUS_LEXERNULL);
}

static t_status	lexer_init(t_lexer **lexer, t_minishell *ms)
{
	t_status	status;

	if (lexer)
	{
		(*lexer) = (t_lexer *)minishell_calloc(1, sizeof(t_lexer));
		if (!*lexer)
			return (STATUS_MALLOCERR);
		ms->lexer = *lexer;
		(*lexer)->cmdline = ms->cmdline;
		status = lexer_cmd(*lexer);
		if (status)
			return (status);
		return (STATUS_SUCCESS);
	}
	return (STATUS_LINITERROR);
}

t_status	minishell_lexer(t_minishell *minishell)
{
	t_lexer		*lexer;
	t_status	status;

	/*
		BUG1 : handle arg string case where `"something""something"` is one arg;
	*/
	status = lexer_init(&lexer, minishell);
	if (status)
		return (status);
	status = lexer_lex(lexer);
	if (status)
		return (status);
	status = lexer_validate(lexer->token);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
}
