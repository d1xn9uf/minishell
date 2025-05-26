/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:00:27 by mzary             #+#    #+#             */
/*   Updated: 2025/05/26 07:59:45 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static void		detect_new(t_substr *node);
static bool		count_ast(t_substr *node, uint32_t *a_si);
static t_status	fill_token(t_token *curr, t_token *prev, t_token *last,
	uint32_t a_si);
static t_status	replace_value(t_token *token, t_substr *head);

t_status	separate(t_token *token)
{
	t_substr	*head;
	t_substr	*node;
	uint32_t	a_si;
	t_token		*curr;
	t_token		*last;
	t_token 	*prev;

	(setuint32(&a_si, 0), detect_new(token->subs));
	prev = NULL;
	last = token->right;
	head = token->subs;
	while (head)
	{
		node = head->next;
		if (count_ast(head, &a_si) && head == token->subs)
			curr = token;
		else
			curr = (t_token *)minishell_calloc(1, sizeof(t_token));
		if (fill_token(curr, prev, last, a_si) || replace_value(curr, head))
			return (STATUS_MALLOCERR);
		prev = curr;
		head = node;
	}
	return (STATUS_SUCCESS);
}

static void	detect_new(t_substr *node)
{
	while (node && node->next)
	{
		if (!node->q_type && !node->next->q_type)
			node->next->new_token = true;
		node = node->next;
	}
}

static bool		count_ast(t_substr *node, uint32_t *a_si)
{
	while (node && !node->new_token)
	{
		*a_si += minishell_count(node->value, '*');
		node = node->next;
	}
	return (true);
}

static t_status	fill_token(t_token *curr, t_token *prev, t_token *last, uint32_t a_si)
{
	if (!curr)
		return (STATUS_MALLOCERR);
	if (prev)
	{
		curr->ast_flags = prev->ast_flags;
		curr->a_si = a_si;
		curr->right = last;
		prev->right = curr;
	}
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
