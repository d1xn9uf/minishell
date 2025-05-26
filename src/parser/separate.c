/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:53 by mzary             #+#    #+#             */
/*   Updated: 2025/05/26 11:19:24 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	separate_subs(t_substr *head);
static void		detect_new(t_substr *head, bool split);
static bool		space_at(char *value, uint8_t end);
static t_status	separate_sub(t_substr *node, char **splits);

t_status	minishell_separate(t_token *token, bool sep)
{
	if (sep && separate_subs(token->subs))
		return (STATUS_MALLOCERR);
	if (minishell_remove(token))
		return (STATUS_MALLOCERR);
	if (sep && separate(token))
		return (STATUS_MALLOCERR);
	else if (!sep && replace_value(token, token->subs))
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_status	separate_subs(t_substr *head)
{
	char		**splits;
	t_substr	*node;

	detect_new(head, false);
	node = head;
	while (node)
	{
		if (!node->q_type)
		{
			splits = minishell_split(node->value, SPACE, NULL);
			if (!splits)
				return (STATUS_MALLOCERR);
			if (splits[0] && separate_sub(node, splits))
				return (minishell_free_arr(splits), STATUS_MALLOCERR);
			minishell_free_arr(splits);
		}
		node = node->next;
	}
	detect_new(head, true);
	return (STATUS_SUCCESS);
}

static void	detect_new(t_substr *head, bool split)
{
	while (head && head->next)
	{
		if (!split && head->q_type && !head->next->q_type
			&& space_at(head->next->value, 0))
			head->next->new_token = true;
		else if (!split && !head->q_type && head->next->q_type
			&& space_at(head->next->value, 1))
			head->next->new_token = true;
		else if (split && !head->q_type && !head->next->q_type)
			head->next->new_token = true;
		head = head->next;
	}
}

static bool	space_at(char *value, uint8_t end)
{
	uint32_t	i;

	if (end == 0)
	{
		i = 0;
		while (value[i] && minishell_isspace(value[i]))
			i += 1;
		return (i != 0);
	}
	else
	{
		i = minishell_strlen(value) - 1;
		while (i >= 0 && minishell_isspace(value[i]))
			i -= 1;
		return (value[i + 1] != 0);
	}
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
