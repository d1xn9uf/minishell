/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:53 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 19:30:09 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	separate(t_token *token, char **splits);

t_status	minishell_separate(t_token *token)
{
	char		**splits;
	t_status	status;

	if (*token->tvalue)
	{
		splits = minishell_split(token->tvalue, SPACE, NULL);
		if (!splits)
			return (STATUS_MALLOCERR);
		status = separate(token, splits);
		minishell_free_arr(splits);
		if (status)
			return (status);
	}
	return (STATUS_SUCCESS);
}

static t_status	separate(t_token *token, char **splits)
{
	t_token		*last_right;
	char		*value;
	uint32_t	i;

	last_right = token->right;
	value = minishell_strdup(splits[0]);
	if (!value)
		return (STATUS_MALLOCERR);
	minishell_free((void **)&token->tvalue);
	token->tvalue = value;
	i = 0;
	while (setbool(&token->is_interpreted, true) && splits[++i])
	{
		token->right = (t_token *)minishell_calloc(1, sizeof(t_token));
		if (!token->right)
			return (token->right = last_right, STATUS_MALLOCERR);
		token->right->tvalue = minishell_strdup(splits[i]);
		if (!token->right->tvalue && minishell_free((void **)&token->right))
			return (token->right = last_right, STATUS_MALLOCERR);
		token->right->ttype = TTOKEN_ARGUMENT;
		token->right->ast_flags = token->ast_flags;
		token->right->a_si = token->a_si + minishell_count(token->tvalue, '*');
		token = token->right;
	}
	return (token->right = last_right, STATUS_SUCCESS);
}
