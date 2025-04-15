/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:56 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:33:56 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	interpret_dollar(t_token *token, t_env *env, t_args args);
static t_status	interpret_asterisk(t_token *token);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args)
{
	t_status	status;

	if (args.step == 0)
	{
		status = interpret_dollar(token, env, args);
		if (status)
			return (status);
		status = minishell_separate(token);
		if (status)
			return (status);
	}
	if (args.step == 1)
	{
		if (args.flag)
		{
			status = interpret_asterisk(token);
			if (status)
				return (status);
		}
		status = minishell_remove(token);
		if (status)
			return (status);
	}
	return (STATUS_SUCCESS);
}

static t_status	interpret_dollar(t_token *token, t_env *env, t_args args)
{
	char	*e_value;

	if (minishell_strchr(token->tvalue, '$'))
	{
		e_value = minishell_expand(token->tvalue, env, args);
		if (!e_value)
			return (STATUS_MALLOCERR);
		minishell_free((void **)&token->tvalue);
		token->tvalue = e_value;
	}
	return (STATUS_SUCCESS);
}

static void		decide_asterisk(char *value, bool *asterisk);

static t_status	interpret_asterisk(t_token *token)
{
	bool		*asterisk;
	t_status	status;
	uint32_t	len;

	if (minishell_strchr(token->tvalue, '*'))
	{
		len = minishell_strlen(token->tvalue);
		asterisk = (bool *)malloc(sizeof(bool) * len);
		if (!asterisk)
			return (STATUS_MALLOCERR);
		decide_asterisk(token->tvalue, asterisk);
		status = minishell_asterisk(token, asterisk);
		minishell_free((void **)&asterisk);
		if (status)
			return (status);
	}
	return (STATUS_SUCCESS);
}

static void	decide_asterisk(char *value, bool *asterisk)
{
	uint32_t	i;
	bool		flag[2];

	i = 0;
	flag[0] = false;
	flag[1] = false;
	while (value[i])
	{
		if (*value == CHAR_DOUBLE_QUOTE && !flag[0])
			flag[1] = !flag[1];
		else if (*value == CHAR_SINGLE_QUOTE && !flag[1])
			flag[0] = !flag[0];
		asterisk[i] = (!flag[0] && !flag[1]);
		i += 1;
	}
}

bool	setbool(bool *flag, bool value)
{
	if (flag)
		*flag = value;
	return (true);
}
