/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:27 by mzary             #+#    #+#             */
/*   Updated: 2025/05/25 20:42:31 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/executor.h"

void	exec_failed(t_root *cmd_node, int32_t status)
{
	if (status == STATUS_CMDNOTFOUND)
	{
		while (cmd_node && !cmd_node->tvalue[0] && cmd_node->is_interpreted)
			cmd_node = cmd_node->right;
		if (cmd_node)
		{
			minishell_stderr("minishell: ", cmd_node->tvalue,
				": command not found\n");
		}
	}
}

static void	exec_exec(t_minishell *minishell, char **argv)
{
	int32_t	status;
	char	**envp;
	pid_t	forked;

	forked = fork();
	minishell_sigstatus(true, -1);
	if (forked == CHILD_PROCESS)
	{
		signal(SIGQUIT, SIG_DFL);
		envp = minishell_getenvp(minishell->env);
		if (!envp)
			exit(EXIT_FAILURE);
		execve(argv[0], argv, envp);
		cleanup_fds(minishell, true);
		minishell_free_arr(envp);
		exit(EXIT_FAILURE);
	}
	waitpid(forked, &status, 0);
	minishell_sigstatus(true, 0);
	minishell_setstatus(minishell, status);
}

void	exec_cmd(t_minishell *minishell, t_root *cmd_node)
{
	char	**argv;
	int32_t	status;

	status = 0;
	if (!cmd_node->tvalue)
		return ;
	argv = executor_getargs(cmd_node, minishell, (t_status *)&status);
	if (!argv)
	{
		exec_failed(cmd_node, status);
		return ;
	}
	if (minishell_isbuiltin(argv[0]))
		minishell->exit_code = exec_builtin(minishell, argv);
	else
		exec_exec(minishell, argv);
	minishell_free((void **)&argv);
}
