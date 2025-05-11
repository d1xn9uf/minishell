/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:29 by mzary             #+#    #+#             */
/*   Updated: 2025/05/08 13:46:09 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

pid_t	g_sig_pid = 42;

t_status	minishell_init(t_minishell **minishell, char **env)
{
	if (minishell)
	{
		*minishell = (t_minishell *)minishell_calloc(1, sizeof(t_minishell));
		if (!minishell)
			return (STATUS_MALLOCERR);
		(*minishell)->prompt = PROMPT;
		(*minishell)->env = minishell_getenv(env);
		(*minishell)->cwd = minishell_getvalue((*minishell)->env, "$PWD");
		if (!(*minishell)->cwd)
			return (STATUS_MALLOCERR);
		(*minishell)->stdfd[0] = dup(STDIN_FILENO);
		(*minishell)->stdfd[1] = dup(STDOUT_FILENO);
		if (minishell_siginit())
			return (STATUS_SIGINIT);
		if (g_sig_pid == 42 && tcgetattr(STDIN_FILENO, &(*minishell)->original_termios) == -1)
		{
			perror("tcgetattr");
			return (STATUS_TERMIOSAVE);
		}
		g_sig_pid = 1;
		return (STATUS_SUCCESS);
	}
	return (STATUS_MSINITERROR);
}

static t_status	minishell(t_minishell *minishell)
{
	t_status	status;

	minishell->cmdline = readline(minishell->prompt);
	if (!minishell->cmdline)
	{
		minishell_cleanup(minishell, STATUS_SUCCESS);
		exit(STATUS_SUCCESS);
	}
	if (!minishell->cmdline[0])
		return (STATUS_EMPTYCMD);
	add_history(minishell->cmdline);
	status = minishell_lexer(minishell);
	if (status)
	{
		if (status == STATUS_SYNTAXERR)
			minishell->exit_code = 2;
		return (minishell_free_token(minishell->lexer->token), status);
	}
	status = minishell_parser(minishell);
	if (status)
		return (status);
	status = minishell_executor(minishell);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_minishell	*ms;
	t_status	status;

	(void)av;
	(void)ac;
	status = minishell_init(&ms, env);
	if (status)
	{
		minishell_error(status);
		minishell_reset(ms);
		return (STATUS_FAILURE);
	}
	while (true)
	{
		status = minishell(ms);
		if (status == STATUS_EMPTYCMD)
			continue ;
		if (status)
			minishell_error(status);
		minishell_reset(ms);
	}
	return (STATUS_SUCCESS);
}
