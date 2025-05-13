/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:15 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:15 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/lexer.h"

static void	cmd_split_size(const char *s, uint64_t *count)
{
	bool	flag;
	char	quote;

	flag = false;
	*count = 0;
	while (*s)
	{
		if (*s == CHAR_SINGLE_QUOTE || *s == CHAR_DOUBLE_QUOTE)
		{
			quote = *s++;
			while (*s && *s != quote)
				s++;
			s++;
			if (!flag && (!*s || *s == SPACE))
				(*count)++;
			continue ;
		}
		if (*s != SPACE && !flag)
		{
			(*count)++;
			flag = true;
		}
		else if (*s == SPACE)
			flag = false;
		s++;
	}
}

void	cmd_split_quoted(char **spaced, t_lexer *lexer,
			uint32_t element)
{
	char		quote;
	uint64_t	len;

	len = 1;
	quote = *(*spaced);
	while (*((*spaced) + len) != quote)
		len++;
	len += 1;
	if (*((*spaced) + len) && *((*spaced) + len) != SPACE)
	{
		while (*((*spaced) + len) && *((*spaced) + len) != SPACE)
		{
			if (*((*spaced) + len) == CHAR_SINGLE_QUOTE
				|| *((*spaced) + len) == CHAR_DOUBLE_QUOTE)
			{
				quote = *((*spaced) + len++);
				while (*((*spaced) + len) != quote)
					len++;
			}
			len++;
		}
	}
	lexer->splited_cmdline[element] = (char *)minishell_calloc(len + 1, 1);
	minishell_strlcpy(lexer->splited_cmdline[element], *(spaced), len + 1);
	*spaced += len;
}

static t_status	cmd_split_word(char **spaced, t_lexer *lexer,
	uint32_t element)
{
	uint64_t	len;
	char		quote;

	if (**spaced == CHAR_SINGLE_QUOTE || **spaced == CHAR_DOUBLE_QUOTE)
		return (cmd_split_quoted(spaced, lexer, element), STATUS_SUCCESS);
	len = 0;
	while (*((*spaced) + len) && *((*spaced) + len) != SPACE)
	{
		if (*((*spaced) + len) == CHAR_SINGLE_QUOTE
			|| *((*spaced) + len) == CHAR_DOUBLE_QUOTE)
		{
			quote = *((*spaced) + len++);
			while (*((*spaced) + len) != quote)
				len++;
		}
		len++;
	}
	lexer->splited_cmdline[element] = (char *)minishell_calloc(len + 1, 1);
	if (!lexer->splited_cmdline[element])
		return (STATUS_MALLOCERR);
	minishell_strlcpy(lexer->splited_cmdline[element], *spaced, len + 1);
	*spaced += len;
	return (STATUS_SUCCESS);
}

static t_status	init_split(t_lexer *lexer)
{
	uint64_t	size;

	cmd_split_size(lexer->spaced_cmdline, &size);
	lexer->splited_cmdline = (char **)minishell_calloc(size + 1, sizeof(char *));
	if (!lexer->splited_cmdline)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

t_status	lexer_cmd_split(t_lexer *lexer)
{
	t_status	status;
	uint32_t	count;
	char		*spaced;

	status = init_split(lexer);
	if (status)
		return (status);
	count = 0;
	spaced = lexer->spaced_cmdline;
	while (*spaced)
	{
		while (minishell_isspace(*spaced))
			spaced++;
		if (!*spaced)
			break ;
		status = cmd_split_word(&spaced, lexer, count);
		if (status)
			return (status);
		count++;
	}
	lexer->splited_cmdline[count] = 0;
	lexer->sztoken = count;
	return (STATUS_SUCCESS);
}
