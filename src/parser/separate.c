/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:53 by mzary             #+#    #+#             */
/*   Updated: 2025/05/25 18:42:06 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	separate_subs(t_substr *head);
static t_status	separate_sub(t_substr *node, char **splits);
static t_status	separate(t_token *token, t_substr *head, uint32_t a_si);
static t_status	replace_value(t_token *token, t_substr *head);

t_status	minishell_separate(t_token *token, bool sep)
{
	if (sep && separate_subs(token->subs))
		return (STATUS_MALLOCERR);
	if (minishell_remove(token) || separate(token, token->subs, 0))
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

static t_status	separate(t_token *token, t_substr *head, uint32_t a_si)
{
	t_substr	*node;
	t_token		*new;

	node = head;
	while (node && node->next)
	{
		a_si += minishell_count(node->value, '*');
		if (!node->q_type && !node->next->q_type)
		{
			node->next->new_token = true;
			new = (t_token *)minishell_calloc(1, sizeof(t_token));
			if (!new || separate(new, node->next, a_si))
				return (STATUS_MALLOCERR);
			if (!new->right)
				new->right = token->right;
			new->ast_flags = token->ast_flags;
			new->a_si = a_si;
			token->right = new;
			break ;
		}
		node = node->next;
	}
	if (replace_value(token, head))
			return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_status	replace_value(t_token *token, t_substr *head)
{
	char	*new_value;

	new_value = minishell_concatenate(head);
	if (!new_value)
		return (STATUS_MALLOCERR);
	if (token->tvalue)
		minishell_free((void **)&token->tvalue);
	token->tvalue = new_value;
	token->is_interpreted = true;
	return (STATUS_SUCCESS);
}
