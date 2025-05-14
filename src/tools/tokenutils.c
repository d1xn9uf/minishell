/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenutils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:50:08 by mzary             #+#    #+#             */
/*   Updated: 2025/05/14 15:50:10 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	minishell_iscmdop(t_token *token)
{
	if (token->ttype == TTOKEN_AND_OP
		|| token->ttype == TTOKEN_OR_OP
		|| token->ttype == TTOKEN_PIPE)
		return (true);
	return (false);
}

uint32_t	minishell_getokensize(t_token *token)
{
	uint32_t	size;

	size = 0;
	if (token)
	{
		while (token)
		{
			size++;
			token = token->next_token;
		}
		return (size);
	}
	return (0);
}

static void	move(t_token **head, t_token *src, t_token *dst)
{
	uint32_t	new_tid;
	t_token		*current;

	if (src->prev_token != NULL)
		src->prev_token->next_token = src->next_token;
	else
		*head = src->next_token;
	if (src->next_token != NULL)
		src->next_token->prev_token = src->prev_token;
	src->next_token = dst;
	src->prev_token = dst->prev_token;
	if (dst->prev_token != NULL)
		dst->prev_token->next_token = src;
	else
		*head = src;
	dst->prev_token = src;
	current = *head;
	new_tid = 0;
	while (current != NULL)
	{
		current->tid = new_tid++;
		current = current->next_token;
	}
}

void	move_token(t_token **head, uint32_t stid, uint32_t dtid)
{
	t_token		*src;
	t_token		*dst;
	t_token		*current;

	if (*head == NULL || stid == dtid)
		return ;
	src = NULL;
	dst = NULL;
	current = *head;
	while (current != NULL)
	{
		if (current->tid == stid)
			src = current;
		else if (current->tid == dtid)
			dst = current;
		if (src != NULL && dst != NULL)
			break ;
		current = current->next_token;
	}
	if (src == NULL || dst == NULL)
		return ;
	move(head, src, dst);
}
