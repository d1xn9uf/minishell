/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:21 by mzary             #+#    #+#             */
/*   Updated: 2025/05/19 14:53:05 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/executor.h"

static void	setup_input_output(t_root *node,
		int32_t input_fd, int32_t output_fd)
{
	if (input_fd != 0 || node->hd.is_hd)
	{
		if (node->hd.is_hd && dup2(node->hd.fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		else if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		if (dup2(output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(output_fd);
	}
}

static void	exec_redirect_if_needed(t_minishell *minishell, t_root *node,
		int32_t input_fd, int32_t output_fd)
{
	t_root	*rnode;

	rnode = NULL;
	if (minishell_isred(node) || minishell_isred(node->left))
	{
		if (minishell_isred(node))
			rnode = node;
		else
			rnode = node->left;
		exec_redirect(minishell, rnode, input_fd, output_fd);
		cleanup_fds(minishell, true);
		exit(minishell->exit_code);
	}
	cleanup_fds(minishell, true);
}

static void	pipeit_child(t_minishell *minishell, t_root *node, int32_t input_fd,
		int32_t output_fd)
{
	t_norm_pipe	p;

	signal(SIGQUIT, SIG_DFL);
	p.cmd_node = node->left;
	if (node->ttype == TTOKEN_COMMAND)
		p.cmd_node = node;
	setup_input_output(p.cmd_node, input_fd, output_fd);
	exec_redirect_if_needed(minishell, node, 0, 1);
	p.status = 0;
	p.argv = executor_getargs(p.cmd_node, minishell, &p.status);
	if (!p.argv)
	{
		exec_failed(minishell, p.cmd_node, p.status);
		exit(minishell->exit_code);
	}
	if (minishell_isbuiltin(p.argv[0]))
		exit(exec_builtin(minishell, p.argv));
	else
	{
		p.envp = minishell_getenvp(minishell->env);
		if (p.envp)
			execve(p.argv[0], p.argv, p.envp);
		minishell_free_arr(p.envp);
	}
	(minishell_free((void **)&p.argv), exit(STATUS_FAILURE));
}

static void	handle_parent(t_minishell *minishell, t_root *node,
		int32_t pipe_fd[2], int32_t info[])
{
	int32_t	status;

	if (info[0] != 0)
		close(info[0]);
	if (node->ttype == TTOKEN_PIPE)
		close(pipe_fd[PIPE_WRITE_END]);
	if (node->ttype == TTOKEN_PIPE)
		pipeit(minishell, node->right, pipe_fd[PIPE_READ_END]);
	waitpid(info[1], &status, 0);
	if (!minishell->pipe_exit)
	{
		minishell_setstatus(minishell, status);
		minishell->pipe_exit = true;
	}
}

void	pipeit(t_minishell *ms, t_root *node, int32_t input_fd)
{
	t_norm_pipe2	var;

	if (node == NULL)
		return ;
	if (node->ttype == TTOKEN_PIPE && pipe(var.pipe_fd) == -1)
	{
		perror("pipe");
		return ;
	}
	var.forked = fork();
	if (minishell_sigstatus(true, -1) && var.forked == CHILD_PROCESS)
	{
		if (node->ttype == TTOKEN_PIPE)
			close(var.pipe_fd[PIPE_READ_END]);
		if (node->ttype == TTOKEN_PIPE)
			pipeit_child(ms, node, input_fd, var.pipe_fd[PIPE_WRITE_END]);
		pipeit_child(ms, node, input_fd, ms->stdfd[1]);
	}
	else if (var.forked > 0)
	{
		var.info[0] = input_fd;
		var.info[1] = var.forked;
		handle_parent(ms, node, var.pipe_fd, var.info);
	}
}
