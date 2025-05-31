/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:29 by mzary             #+#    #+#             */
/*   Updated: 2025/05/30 13:50:20 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	ctrl_d_exit(t_minishell *minishell)
{
	if (minishell_sigstatus(false, 0) == SIGINT && minishell_sigstatus(true, 0))
	{
		minishell->exit_code = 130;
	}
	minishell_stderr("exit\n", NULL, NULL);
	minishell_cleanup(minishell, minishell->exit_code);
}

static t_status	minishell_init(t_minishell **minishell, char **env)
{
	if (minishell)
	{
		*minishell = (t_minishell *)minishell_calloc(1, sizeof(t_minishell));
		if (!minishell)
			return (STATUS_MALLOCERR);
		(*minishell)->prompt = PROMPT;
		(*minishell)->env = minishell_getenv(env);
		(*minishell)->cwd = getcwd(NULL, 0);
		if (!(*minishell)->cwd)
		{
			minishell_stderr(
				"minishell_init: error: getcwd: could not get cwd\n",
				NULL, NULL);
			(*minishell)->cwd = minishell_strdup("");
			if (!(*minishell)->cwd)
				return (STATUS_MALLOCERR);
		}
		(*minishell)->stdfd[0] = dup(STDIN_FILENO);
		(*minishell)->stdfd[1] = dup(STDOUT_FILENO);
		if (minishell_siginit())
			return (STATUS_SIGINIT);
		return (STATUS_SUCCESS);
	}
	return (STATUS_MSINITERROR);
}

static t_status	minishell(t_minishell *minishell)
{
	t_status	status;

	minishell->cmdline = readline(minishell->prompt);
	if (!minishell->cmdline)
		ctrl_d_exit(minishell);
	if (minishell_sigstatus(false, 0) == SIGINT
		&& minishell_sigstatus(true, 0))
		minishell->exit_code = 130;
	if (!minishell->cmdline[0])
		return (STATUS_EMPTYCMD);
	add_history(minishell->cmdline);
	status = minishell_lexer(minishell);
	if (status)
		return (minishell_free_token(minishell->lexer->token), status);
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
		minishell_cleanup(ms, status);
	}
	while (true)
	{
		status = minishell(ms);
		if (status == STATUS_EMPTYCMD)
			continue ;
		else if (status == STATUS_SYNTAXERR)
			ms->exit_code = 2;
		if (status)
			minishell_error(status);
		minishell_reset(ms);
	}
	return (STATUS_SUCCESS);
}
