#include "../../inc/lexer.h"

static t_status	cmd_spaced_len(char *cmdline, uint64_t *sz)
{
	while (*cmdline)
	{
		if (minishell_iscmdsep(*cmdline))
		{
			if (!minishell_iscmdsep(*(cmdline - 1)))
				(*sz)++;
			else if (*cmdline != *(cmdline - 1))
				(*sz)++;
		}
		else
		{
			if (minishell_iscmdsep(*(cmdline - 1)))
				(*sz)++;
		}
		(*sz)++;
		cmdline++;
	}
	return (STATUS_SUCCESS);
} 	

static bool	cmd_ignore_quotes(char **cmdline, char **spaced_cmdline)
{
	char	quote;

	if (**cmdline == CHAR_SINGLE_QUOTE || **cmdline == CHAR_DOUBLE_QUOTE)
	{
		quote = **cmdline;
		*(*spaced_cmdline)++ = *(*cmdline)++;
		while (**cmdline && **cmdline != quote)
			*(*spaced_cmdline)++ = *(*cmdline)++;
		if (!**cmdline)
			return (false);
		return (true);
	}
	return (true);
}

static t_status	cmd_spaced(char **cmdline, char **spaced_cmdline, bool cmdissep)
{
	if (!cmdissep)
	{
		if (minishell_iscmdsep(*((*cmdline) - 1)))
			*(*spaced_cmdline)++ = SPACE;
		if (!cmd_ignore_quotes(cmdline, spaced_cmdline))
			return (STATUS_SYNTAXERR);
		*(*spaced_cmdline)++ = *(*cmdline)++;
	}
	else
	{
		if (minishell_iscmdsep(*((*cmdline) - 1))
			&& **cmdline == *((*cmdline) - 1))
			*(*spaced_cmdline)++ = *(*cmdline)++;
		else
		{
			if (*((*cmdline) - 1) != SPACE)
				*(*spaced_cmdline)++ = SPACE;
			*(*spaced_cmdline)++ = *(*cmdline)++;
		}
	}
	return (STATUS_SUCCESS);
}

static t_status	init_spaced(t_lexer *lexer)
{
	cmd_spaced_len(lexer->cmdline, &lexer->spaced.sz);
	if (lexer->spaced.sz == 0)
		return (STATUS_EMPTYCMD);
	lexer->spaced.spaced_cmdline = (char *)malloc(lexer->spaced.sz + 1);
	if (!lexer->spaced.spaced_cmdline)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}	

t_status	lexer_cmd_spaced(t_lexer *lexer)
{
	t_status	status;
	char		*cmdline;
	char		*spaced_cmdline;

	status = init_spaced(lexer);
	if (status)
		return (status);
	cmdline = lexer->cmdline;
	spaced_cmdline = lexer->spaced.spaced_cmdline;
	while (*cmdline)
	{
		if (!minishell_iscmdsep(*cmdline))
		{
			status = cmd_spaced(&cmdline, &spaced_cmdline, false);
			if (status)
				return (status);
		}
		else
			cmd_spaced(&cmdline, &spaced_cmdline, true);
	}
	*spaced_cmdline = 0;
	return (STATUS_SUCCESS);
}
