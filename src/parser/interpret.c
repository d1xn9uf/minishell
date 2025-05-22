/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:56 by mzary             #+#    #+#             */
/*   Updated: 2025/05/22 21:46:48 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_substr	*get_subs(char *s);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args)
{
	t_status	status;

	token->interp.subs = get_subs(token->tvalue);
	if (!token->interp.subs)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_substr	*get_subs(char *s)
{
	static t_substr	*head;
	char			q;
	uint32_t		i;
	t_substr		*node;

	if (!*s)
		return (NULL);
	i = 0;
	q = s[0];
	while (s[i])
	{
		if ((s[i] == CHAR_SINGLE_QUOTE && q != CHAR_DOUBLE_QUOTE)
			|| (s[i] == CHAR_DOUBLE_QUOTE && q != CHAR_SINGLE_QUOTE))
			break ;
		i += 1;
	}
	i += (q == s[i]);
	node = (t_substr *)minishell_calloc(1, sizeof(t_substr));
	if (!node)
		return (minishell_freesubs(head), NULL);
	node->q_type = s[i] * (q == CHAR_DOUBLE_QUOTE || q == CHAR_SINGLE_QUOTE);
	node->value = minishell_substr(s, 0, i);
	if (!node->value && minishell_free((void **)&node))
		return (minishell_freesubs(head), NULL);
	node->next = get_subs(s + i);
	return (head);
}
