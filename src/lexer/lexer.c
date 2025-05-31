/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:16 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:17 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	t_status	status2;
	int32_t		hdoc_count;

	status = lexer_init(&lexer, minishell);
	if (status)
		return (status);
	status = lexer_lex(lexer);
	if (status)
		return (status);
	hdoc_count = 0;
	status2 = 0;
	status = lexer_validate(lexer->token, &status2, &hdoc_count);
	if (status)
	{
		if (status2 == STATUS_TOMANYHDOC)
		{
			minishell_free_token(minishell->lexer->token);
			minishell_cleanup(minishell, 2);
		}
		return (status);
	}
	return (STATUS_SUCCESS);
}
