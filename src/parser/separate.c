/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:53 by mzary             #+#    #+#             */
/*   Updated: 2025/05/26 07:16:32 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	separate_subs(t_substr *head);
static t_status	separate_sub(t_substr *node, char **splits);

t_status	minishell_separate(t_token *token, bool sep)
{
	if (sep && separate_subs(token->subs))
		return (STATUS_MALLOCERR);
	if (minishell_remove(token) || separate(token))
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_status	separate_subs(t_substr *head)
{
	char	**splits;

	while (head)
	{
		if (!head->q_type)
		{
			splits = minishell_split(head->value, SPACE, NULL);
			if (!splits)
				return (STATUS_MALLOCERR);
			if (splits[0] && separate_sub(head, splits))
				return (minishell_free_arr(splits), STATUS_MALLOCERR);
			minishell_free_arr(splits);
		}
		head = head->next;
	}
	return (STATUS_SUCCESS);
}

static t_status	separate_sub(t_substr *node, char **splits)
{
	uint32_t	i;
	t_substr	*old_next;
	t_substr	*new_next;
	char		*value;

	i = 0;
	value = minishell_strdup(splits[0]);
	if (!value)
		return (STATUS_MALLOCERR);
	minishell_free((void **)&node->value);
	node->value = value;
	old_next = node->next;
	while (splits[++i])
	{
		new_next = (t_substr *)minishell_calloc(1, sizeof(t_substr));
		if (!new_next)
			return (STATUS_MALLOCERR);
		new_next->value = minishell_strdup(splits[i]);
		if (!new_next->value && minishell_free((void **)&new_next))
			return (STATUS_MALLOCERR);
		node->next = new_next;
		node->next->next = old_next;
		node = node->next;
	}
	return (STATUS_SUCCESS);
}

