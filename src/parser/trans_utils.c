/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trans_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:00:14 by mzary             #+#    #+#             */
/*   Updated: 2025/05/22 14:12:44 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	fill_dollar(t_token *token);
static t_status	fill_asterisk(t_token *token);

t_status	minishell_fillflags(t_token *token)
{
	uint32_t	j;

	if (!token || !token->tvalue)
		return (STATUS_SUCCESS);
	token->d.n = 0;
	token->a.n = 0;
	j = 0;
	while (token->tvalue[j])
	{
		token->d.n += (token->tvalue[j] == CHAR_DOLLAR_SIGN);
		token->a.n += (token->tvalue[j] == CHAR_ASTERISK);
		j += 1;
	}

	if (fill_dollar(token))
		return (STATUS_MALLOCERR);
	if (fill_asterisk(token))
		return (minishell_free((void **)&token->d.flags),
			STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_status	fill_dollar(t_token *token)
{
	bool		q[2];
	uint32_t	j;

	token->d.flags = (bool *)minishell_calloc(token->d.n, sizeof(bool));
	if (!token->d.flags)
		return (STATUS_MALLOCERR);
	minishell_memset(q, 0, 2 * sizeof(bool));
	j = 0;
	token->d.i = 0;
	while (token->tvalue[j])
	{
		if (token->tvalue[j] == CHAR_DOUBLE_QUOTE && !q[0])
			q[1] = !q[1];
		else if (token->tvalue[j] == CHAR_SINGLE_QUOTE && !q[1])
			q[0] = !q[0];
		else if (token->tvalue[j] == CHAR_DOLLAR_SIGN)
		{
			token->d.flags[token->d.i] = !q[0];
			token->d.i += 1;
		}
		j += 1;
	}
	return (STATUS_SUCCESS);
}

static t_status	fill_asterisk(t_token *token)
{
	bool		q[2];
	uint32_t	j;

	token->a.flags = (bool *)minishell_calloc(token->a.n, sizeof(bool));
	if (!token->a.flags)
		return (STATUS_MALLOCERR);
	minishell_memset(q, 0, 2 * sizeof(bool));
	j = 0;
	token->a.i = 0;
	while (token->tvalue[j])
	{
		if (token->tvalue[j] == CHAR_DOUBLE_QUOTE && !q[0])
			q[1] = !q[1];
		else if (token->tvalue[j] == CHAR_SINGLE_QUOTE && !q[1])
			q[0] = !q[0];
		else if (token->tvalue[j] == CHAR_ASTERISK)
		{
			token->a.flags[token->a.i] = !q[0] && !q[1];
			token->a.i += 1;
		}
		j += 1;
	}
	return (STATUS_SUCCESS);
}

t_status	update_command(t_token *token, t_env *env)
{
	t_status	s;
	char		*temp;

	if (token->is_expanded && !token->tvalue[0] && token->right)
		token->right->ttype = TTOKEN_COMMAND;
	else
	{
		s = minishell_remove(token);
		if (s)
			return (s);
		temp = minishell_getpath(env, token->tvalue, &s);
		if (!temp)
			return (s);
		minishell_free((void **)&token->tvalue);
		token->tvalue = temp;
	}
	return (STATUS_SUCCESS);
}
