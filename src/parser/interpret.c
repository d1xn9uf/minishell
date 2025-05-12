/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:56 by mzary             #+#    #+#             */
/*   Updated: 2025/05/12 20:05:24 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	interpret_dollar(t_token *token, t_env *env, t_args args);
static t_status	interpret_asterisk(t_token *token);
static bool		*get_asterisk(char *value);
static void		decide_asterisk(bool *asterisk, char *value);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args)
{
	t_status	status;

	if (args.step == 0)
	{
		if (minishell_strchr(token->tvalue, '$'))
		{
			status = interpret_dollar(token, env, args);
			if (status)
				return (status);
			status = minishell_separate(token);
			if (status)
				return (status);
		}
	}
	if (args.step == 1)
	{
		if (args.flag && minishell_strchr(token->tvalue, '*')
			&& !token->is_filename)
			status = interpret_asterisk(token);
		else
		{
			printf("salam ana hna\n");
			status = minishell_remove(token);
		}
		if (status)
			return (status);
	}
	return (STATUS_SUCCESS);
}

static t_status	interpret_dollar(t_token *token, t_env *env, t_args args)
{
	char	*e_value;

	e_value = minishell_expand(token->tvalue, env, args);
	if (!e_value)
		return (STATUS_MALLOCERR);
	minishell_free((void **)&token->tvalue);
	token->tvalue = e_value;
	return (STATUS_SUCCESS);
}

static t_status	interpret_asterisk(t_token *token)
{
	bool		*asterisk;
	t_status	status;

	asterisk = get_asterisk(token->tvalue);
	if (!asterisk)
		return (STATUS_MALLOCERR);
	status = minishell_remove(token);
	if (status)
		return (minishell_free((void **)&asterisk), status);
	status = minishell_asterisk(token, asterisk);
	minishell_free((void **)&asterisk);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
}

static bool	*get_asterisk(char *value)
{
	bool		flags[2];
	uint32_t	final_len;
	uint32_t	i;
	bool		*asterisk;

	minishell_memset(flags, 0, 2 * sizeof(bool));
	final_len = 0;
	i = 0;
	while (value[i])
	{
		if (value[i] == CHAR_DOUBLE_QUOTE && !flags[0])
			flags[1] = !flags[1];
		else if (value[i] == CHAR_SINGLE_QUOTE && !flags[1])
			flags[0] = !flags[0];
		else
			final_len += 1;
		i += 1;
	}
	asterisk = minishell_calloc(final_len + 1, sizeof(bool));
	if (!asterisk)
		return (NULL);
	decide_asterisk(asterisk, value);
	return (asterisk);
}

static void	decide_asterisk(bool *asterisk, char *value)
{
	bool		flags[2];
	uint32_t	i;
	uint32_t	j;

	minishell_memset(flags, 0, 2 * sizeof(bool));
	i = 0;
	j = 0;
	while (value[i])
	{
		if (value[i] == CHAR_DOUBLE_QUOTE && !flags[0])
			flags[1] = !flags[1];
		else if (value[i] == CHAR_SINGLE_QUOTE && !flags[1])
			flags[0] = !flags[0];
		else
		{
			asterisk[j] = false;
			if (value[i] == CHAR_ASTERISK)
				asterisk[j] = (!flags[1] && !flags[0]);
			j += 1;
		}
		i += 1;
	}
}
