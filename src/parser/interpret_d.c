/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret_d.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:45:33 by mzary             #+#    #+#             */
/*   Updated: 2025/05/26 09:45:55 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	expand_subs(t_substr *head, t_env *env, t_args args);
static t_status	get_ast_flags(t_token *token, uint32_t count);
static void		get_sub_flags(bool *ast_flags, uint32_t *a_i, t_substr *node);

t_status	interpret_dollar(t_token *token, t_env *env, t_args *args)
{
	uint32_t	i;
	char		*e_value;
	uint32_t	count;

	i = 0;
	while (token->tvalue[i] && token->tvalue[i] != '=' && !args->flag)
		args->flag = (token->tvalue[i++] == '$');
	if (expand_subs(token->subs, env, *args))
		return (STATUS_MALLOCERR);
	e_value = minishell_concatenate(token->subs);
	if (!e_value)
		return (STATUS_MALLOCERR);
	minishell_free((void **)&token->tvalue);
	token->tvalue = e_value;
	count = minishell_count(token->tvalue, '*');
	if (count && get_ast_flags(token, count))
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_status	expand_subs(t_substr *head, t_env *env, t_args args)
{
	char		*e_sub;
	t_substr	*node;

	node = head;
	while (node)
	{
		e_sub = minishell_expand(node->value, env, args);
		if (!e_sub)
			return (STATUS_MALLOCERR);
		minishell_free((void **)&node->value);
		node->value = e_sub;
		node = node->next;
	}
	return (STATUS_SUCCESS);
}

char	*minishell_concatenate(t_substr *head)
{
	char		*value;
	uint32_t	len;
	t_substr	*node;
	uint32_t	start;

	len = 0;
	node = head;
	while (node && (node == head || !node->new_token))
	{
		len += minishell_strlen(node->value);
		node = node->next;
	}
	value = (char *)minishell_calloc(len + 1, sizeof(char));
	if (!value)
		return (NULL);
	node = head;
	start = 0;
	while (node && (node == head || !node->new_token))
	{
		len = minishell_strlen(node->value);
		minishell_strlcpy(value + start, node->value, len + 1);
		start += len;
		node = node->next;
	}
	return (value);
}

static t_status	get_ast_flags(t_token *token, uint32_t count)
{
	uint32_t	a_i;
	t_substr	*node;

	token->ast_flags = (bool *)minishell_calloc(count, sizeof(bool));
	if (!token->ast_flags)
		return (STATUS_MALLOCERR);
	a_i = 0;
	node = token->subs;
	while (node)
	{
		get_sub_flags(token->ast_flags, &a_i, node);
		node = node->next;
	}
	return (STATUS_SUCCESS);
}

static void	get_sub_flags(bool *ast_flags, uint32_t *a_i, t_substr *node)
{
	uint32_t	i;

	i = 0;
	while (node->value[i])
	{
		if (node->value[i] == '*')
		{
			ast_flags[*a_i] = (node->q_type == 0);
			*a_i += 1;
		}
		i += 1;
	}
}
