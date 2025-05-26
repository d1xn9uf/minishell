/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:00:27 by mzary             #+#    #+#             */
/*   Updated: 2025/05/26 10:29:37 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static void		detect_new(t_substr *node);
static bool		count_ast(t_substr *node, uint32_t *a_si);
static t_status	fill_token(t_norm_sep *tokens, uint32_t a_si);

t_status	separate(t_token *token)
{
	t_substr	*head;
	uint32_t	a_si;
	t_norm_sep	tokens;

	(setuint32(&a_si, 0), detect_new(token->subs));
	tokens.prev = NULL;
	tokens.last = token->right;
	head = token->subs;
	while (head)
	{
		if (count_ast(head, &a_si) && head == token->subs)
			tokens.curr = token;
		else
			tokens.curr = (t_token *)minishell_calloc(1, sizeof(t_token));
		if (fill_token(&tokens, a_si) || replace_value(tokens.curr, head))
			return (STATUS_MALLOCERR);
		tokens.prev = tokens.curr;
		head = head->next;
		while (head && !head->new_token)
			head = head->next;
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

static bool	count_ast(t_substr *node, uint32_t *a_si)
{
	while (node && !node->new_token)
	{
		*a_si += minishell_count(node->value, '*');
		node = node->next;
	}
	return (true);
}

static t_status	fill_token(t_norm_sep *tokens, uint32_t a_si)
{
	if (!tokens->curr)
		return (STATUS_MALLOCERR);
	if (tokens->prev)
	{
		tokens->curr->ast_flags = tokens->prev->ast_flags;
		tokens->curr->a_si = a_si;
		tokens->curr->right = tokens->last;
		tokens->prev->right = tokens->curr;
	}
	return (STATUS_SUCCESS);
}

t_status	replace_value(t_token *token, t_substr *head)
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
