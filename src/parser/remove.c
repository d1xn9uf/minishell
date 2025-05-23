/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:54 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 16:48:41 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	remove_nosubs(t_token *token);
static bool		*get_flags_quotes(char *s);

t_status	minishell_remove(t_token *token)
{
	t_substr	*node;
	t_token		interim;
	char		*e_value;

	if (!token->subs)
		return (remove_nosubs(token));
	node = token->subs;
	while (node)
	{
		interim.tvalue = node->value;
		if (!node->q_type && remove_nosubs(&interim))
			return (STATUS_MALLOCERR);
		node = node->next;
	}
	e_value = minishell_concatenate(token->subs);
	if (!e_value)
		return (STATUS_MALLOCERR);
	minishell_free((void **)&token->tvalue);
	token->tvalue = e_value;
	return (STATUS_SUCCESS);
}

static t_status	remove_nosubs(t_token *token)
{
	t_quotes	vars;

	minishell_memset(&vars, 0, sizeof(t_quotes));
	vars.flags = get_flags_quotes(token->tvalue);
	if (!vars.flags)
		return (STATUS_MALLOCERR);
	while (token->tvalue[vars.i[0]])
		vars.fsize += (vars.flags[vars.i[0]++] == true);
	if (vars.fsize < minishell_strlen(token->tvalue))
	{
		vars.fvalue = (char *)minishell_calloc(vars.fsize + 1, sizeof(char));
		if (!vars.fvalue)
			return (minishell_free((void **)&vars.flags), STATUS_MALLOCERR);
		while (token->tvalue[vars.i[1]])
		{
			if (vars.flags[vars.i[1]++])
				vars.fvalue[vars.i[2]++] = token->tvalue[vars.i[1] - 1];
		}
		vars.fvalue[vars.i[2]] = 0;
		minishell_free((void **)&token->tvalue);
		token->tvalue = vars.fvalue;
	}
	return (minishell_free((void **)&vars.flags), STATUS_SUCCESS);
}

static bool	*get_flags_quotes(char *s)
{
	bool		*qflags;
	uint32_t	len;
	bool		flag[2];
	uint32_t	i;

	len = minishell_strlen(s);
	qflags = (bool *)minishell_calloc(len, sizeof(bool));
	if (!qflags)
		return (NULL);
	minishell_memset(flag, 0, sizeof(bool) * 2);
	i = 0;
	while (i < len)
	{
		qflags[i] = ((s[i] != CHAR_DOUBLE_QUOTE)
				&& (s[i] != CHAR_SINGLE_QUOTE));
		qflags[i] = (qflags[i] || (s[i] == CHAR_DOUBLE_QUOTE && flag[0]));
		qflags[i] = (qflags[i] || (s[i] == CHAR_SINGLE_QUOTE && flag[1]));
		if (s[i] == CHAR_DOUBLE_QUOTE && !flag[0])
			flag[1] = !flag[1];
		else if (s[i] == CHAR_SINGLE_QUOTE && !flag[1])
			flag[0] = !flag[0];
		i += 1;
	}
	return (qflags);
}
