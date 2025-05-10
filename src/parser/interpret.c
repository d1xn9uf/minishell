/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:56 by mzary             #+#    #+#             */
/*   Updated: 2025/05/10 18:13:08 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	interpret_dollar(t_token *token, t_env *env, t_args args);
static t_status	interpret_asterisk(t_token *token);
static bool		*decide_asterisk(char *value);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args)
{
	t_status	status;

	if (args.step == 0
		&& minishell_strchr(token->tvalue, '$'))
	{
		status = interpret_dollar(token, env, args);
		if (status)
			return (status);
		status = minishell_separate(token);
		if (status)
			return (status);
	}
	if (args.step == 1 && args.flag
		&& minishell_strchr(token->tvalue, '*')
		&& !token->is_filename)
	{
		status = interpret_asterisk(token);
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

	asterisk = decide_asterisk(token->tvalue);
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

static bool	*decide_asterisk(char *value)
{
	bool		*asterisk;
	// uint32_t	i;
	// bool		flag[2];

	// i = 0;
	// flag[0] = false;
	// flag[1] = false;
	// while (value[i])
	// {
	// 	if (*value == CHAR_DOUBLE_QUOTE && !flag[0])
	// 		flag[1] = !flag[1];
	// 	else if (*value == CHAR_SINGLE_QUOTE && !flag[1])
	// 		flag[0] = !flag[0];
	// 	asterisk[i] = (!flag[0] && !flag[1]);
	// 	i += 1;
	// }
	return (asterisk);
}
