/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:49 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:50 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static bool	invalid(char *code);

t_status	minishell_exit(t_minishell *minishell, char *code)
{
	int32_t	exit_code;

	write(STDOUT_FILENO, "exit\n", 5);
	if (!code)
		exit_code = minishell->exit_code;
	else if (invalid(code))
		exit_code = 2;
	else
		exit_code = minishell_strtoui8(code);
	minishell_cleanup(minishell, exit_code);
	return (STATUS_SUCCESS);
}

static bool	invalid(char *code)
{
	uint32_t	i;

	i = 0;
	while (minishell_isspace(code[i]))
		i += 1;
	if (code[i] && (code[i] == '+' || code[i] == '-'))
		i += 1;
	while ('0' <= code[i] && code[i] <= '9')
		i += 1;
	while (minishell_isspace(code[i]))
		i += 1;
	if (code[i])
	{
		printf("minishell_exit: %s: numeric argument required\n", code);
		return (true);
	}
	return (false);
}
