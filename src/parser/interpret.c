/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:56 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 18:19:15 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_substr	*get_subs(char *s);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args)
{
	t_status	status;
	uint32_t	sep;

	token->subs = get_subs(token->tvalue);
	if (!token->subs)
		return (STATUS_MALLOCERR);
	status = interpret_dollar(token, env, args);
	if (status && minishell_freesubs(token->subs))
		return (status);
	status = minishell_remove(token);
	if (minishell_freesubs(token->subs) && status)
		return (minishell_free((void **)&token->ast_flags), status);
	status = minishell_separate(token);
	if (status)
		return (minishell_free((void **)&token->ast_flags), status);
	while (token && token->is_interpreted)
	{
		status = interpret_asterisk(token);
		if (status)
			return (minishell_free((void **)&token->ast_flags), status);
		token = token->next_token
	}
	return (minishell_free((void **)&token->ast_flags), STATUS_SUCCESS);
}

static t_substr	*get_subs(char *s)
{
	static t_substr	*head;
	char			q;
	int32_t			i;
	t_substr		*node;

	if (!*s)
		return (NULL);
	i = -1;
	q = s[0];
	while (s[i++])
	{
		if ((s[i] == CHAR_SINGLE_QUOTE && q != CHAR_DOUBLE_QUOTE)
			|| (s[i] == CHAR_DOUBLE_QUOTE && q != CHAR_SINGLE_QUOTE))
			break ;
	}
	i += (q == s[i]);
	node = (t_substr *)minishell_calloc(1, sizeof(t_substr));
	if (!node)
		return (NULL);
	node->q_type = s[i] * (q == CHAR_DOUBLE_QUOTE || q == CHAR_SINGLE_QUOTE);
	node->value = minishell_substr(s, 0, i);
	if (!node->value && minishell_free((void **)&node))
		return (NULL);
	node->next = get_subs(s + i);
	if (!node->next && s[i])
		return (minishell_freesubs(head), NULL);
	return (head);
}
