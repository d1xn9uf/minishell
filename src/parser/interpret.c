/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:56 by mzary             #+#    #+#             */
/*   Updated: 2025/05/24 04:11:16 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_substr	*get_subs(char *s);
static t_substr	*get_substr(char *s, uint32_t *start_i);
static t_status	interpret_ast(t_token *token, bool **ast_flags);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args)
{
	bool	**ast_flags;

	token->subs = get_subs(token->tvalue);
	if (!token->subs)
		return (STATUS_MALLOCERR);
	if (interpret_dollar(token, env, &args) && minishell_freesubs(&token->subs))
		return (STATUS_MALLOCERR);
	ast_flags = &token->ast_flags;
	if (minishell_remove(token))
		return (minishell_free((void **)ast_flags), STATUS_MALLOCERR);
	if (minishell_freesubs(&token->subs) && args.flag && minishell_separate(token))
		return (minishell_free((void **)ast_flags), STATUS_MALLOCERR);
	return (interpret_ast(token, ast_flags));
}

static t_status	interpret_ast(t_token *token, bool **ast_flags)
{
	t_status	status;

	status = STATUS_SUCCESS;
	while (token && token->is_interpreted && !status)
	{
		if (!token->is_asterisked && minishell_strchr(token->tvalue, '*'))
			status = minishell_asterisk(token);
		token = token->next_token;
	}
	return (minishell_free((void **)ast_flags), status);
}

static t_substr	*get_subs(char *s)
{
	t_substr	*head;
	t_substr	*node;
	uint32_t	start_i;

	start_i = 0;
	head = get_substr(s, &start_i);
	if (!head)
		return (NULL);
	node = head;
	while (s[start_i])
	{
		node->next = get_substr(s, &start_i);
		if (!node->next)
			return (minishell_freesubs(&head), NULL);
		node = node->next;
	}
	return (head);
}

static t_substr	*get_substr(char *s, uint32_t *start_i)
{
	t_substr	*node;
	char		q;
	uint32_t	i;

	node = (t_substr *)minishell_calloc(1, sizeof(t_substr));
	if (!node)
		return (NULL);
	q = s[*start_i];
	i = *start_i + 1;
	while (s[i])
	{
		if ((s[i] == CHAR_DOUBLE_QUOTE && q != CHAR_SINGLE_QUOTE)
			|| (s[i] == CHAR_SINGLE_QUOTE && q != CHAR_DOUBLE_QUOTE))
			break ;
		i += 1;
	}
	i += (q == CHAR_DOUBLE_QUOTE || q == CHAR_SINGLE_QUOTE);
	node->q_type = q * (q == CHAR_DOUBLE_QUOTE || q == CHAR_SINGLE_QUOTE);
	node->value = minishell_substr(s, *start_i, i);
	if (!node->value)
		return (minishell_free((void **)&node), NULL);
	*start_i = i;
	return (node);
}
