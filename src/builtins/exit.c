/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:49 by mzary             #+#    #+#             */
/*   Updated: 2025/05/28 09:51:26 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static bool	invalid(char *code);
static bool	is_overflow(char *code, uint32_t *l, bool neg);

t_status	minishell_exit(t_minishell *minishell, char **argv)
{
	int32_t	exit_code;
	bool	inv_arg;

	inv_arg = false;
	if (!argv)
		return (STATUS_FAILURE);
	minishell_stderr("exit\n", NULL, NULL);
	if (!argv[1])
		exit_code = minishell->exit_code;
	else if (invalid(argv[1]))
	{
		minishell_stderr("minishell_exit: ",
			argv[1], ": numeric arg required\n");
		exit_code = 2;
		inv_arg = true;
	}
	else
		exit_code = minishell_strtoui8(argv[1]);
	if (argv[1] && argv[2] && !inv_arg)
		return (minishell_stderr("minishell_exit: too many arguments\n",
				NULL, NULL),
			STATUS_FAILURE);
	minishell_cleanup(minishell, exit_code);
	return (STATUS_SUCCESS);
}

static bool	invalid(char *code)
{
	uint32_t	i;
	uint32_t	l[2];
	bool		neg;

	i = 0;
	neg = false;
	while (minishell_isspace(code[i]))
		i += 1;
	if (code[i] == '+' || code[i] == '-')
	{
		neg = (code[i] == '-');
		i += 1;
	}
	while (code[i] == '0')
		i += 1;
	l[0] = i;
	while ('0' <= code[i] && code[i] <= '9')
		i += 1;
	l[1] = i;
	while (minishell_isspace(code[i]))
		i += 1;
	return (!code[0] || code[i] || is_overflow(code, l, neg));
}

static bool	is_overflow(char *code, uint32_t *l, bool neg)
{
	code[l[1]] = 0;
	if (minishell_strlen(code + l[0]) > minishell_strlen(MIN_SINT64))
		return (true);
	else if (minishell_strlen(code + l[0]) < minishell_strlen(MIN_SINT64))
		return (false);
	else if (neg && minishell_strcmp(code + l[0], MIN_SINT64) > 0)
		return (true);
	else if (!neg && minishell_strcmp(code + l[0], MAX_SINT64) > 0)
		return (true);
	return (false);
}
