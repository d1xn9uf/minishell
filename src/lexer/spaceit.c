/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaceit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:16 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:16 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/lexer.h"

static uint64_t	cmd_spaced_len(char *cmdline)
{
	char		prev_char;
	uint64_t	sz;

	prev_char = 0;
	sz = 0;
	while (*cmdline)
	{
		if (minishell_iscmdsep(*cmdline))
		{
			if (!minishell_iscmdsep(prev_char))
				sz++;
			else if (*cmdline != prev_char)
				sz++;
		}
		else
		{
			if (minishell_iscmdsep(prev_char))
				sz++;
		}
		sz++;
		prev_char = *cmdline;
		cmdline++;
	}
	return (sz);
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

static t_status	cmd_spaced(char **cmdline, char **spaced_cmdline,
					char *prev_char, bool cmdissep)
{
	if (!cmdissep)
	{
		if (minishell_iscmdsep(*prev_char))
			*(*spaced_cmdline)++ = SPACE;
		if (!cmd_ignore_quotes(cmdline, spaced_cmdline))
			return (STATUS_SYNTAXERR);
		*prev_char = **cmdline;
		*(*spaced_cmdline)++ = *(*cmdline)++;
	}
	else
	{
		if (minishell_iscmdsep(*prev_char) && **cmdline == *prev_char)
		{
			*prev_char = **cmdline;
			*(*spaced_cmdline)++ = *(*cmdline)++;
		}
		else
		{
			if (*prev_char != SPACE && *prev_char != 0)
				*(*spaced_cmdline)++ = SPACE;
			*prev_char = **cmdline;
			*(*spaced_cmdline)++ = *(*cmdline)++;
		}
	}
	return (STATUS_SUCCESS);
}

static t_status	init_spaced(t_lexer *lexer, char *init_prev_char)
{
	uint64_t	size;

	size = cmd_spaced_len(lexer->cmdline);
	if (size == 0)
		return (STATUS_EMPTYCMD);
	lexer->spaced_cmdline = (char *)minishell_calloc(1, size + 1);
	if (!lexer->spaced_cmdline)
		return (STATUS_MALLOCERR);
	*init_prev_char = 0;
	return (STATUS_SUCCESS);
}

t_status	lexer_cmd_spaced(t_lexer *lexer)
{
	t_status	status;
	char		*cmdline;
	char		*spaced_cmdline;
	char		prev_char;

	status = init_spaced(lexer, &prev_char);
	if (status)
		return (status);
	cmdline = lexer->cmdline;
	spaced_cmdline = lexer->spaced_cmdline;
	while (*cmdline)
	{
		if (!minishell_iscmdsep(*cmdline))
		{
			status = cmd_spaced(&cmdline, &spaced_cmdline, &prev_char, false);
			if (status)
				return (status);
		}
		else
			cmd_spaced(&cmdline, &spaced_cmdline, &prev_char, true);
	}
	*spaced_cmdline = 0;
	return (STATUS_SUCCESS);
}
