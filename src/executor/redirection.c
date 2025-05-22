/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:21 by mzary             #+#    #+#             */
/*   Updated: 2025/05/16 15:50:29 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/executor.h"

static bool	expand_hdoc_in(t_root *cmd_node, t_env *env, int32_t exit_code)
{
	char	*fdata;
	char	*expanded;
	int32_t	fd;
	t_args	args;

	fdata = minishell_readfile(cmd_node->hd.filename);
	if (!fdata)
		return (false);
	args.exit = minishell_i32tostr(exit_code);
	expanded = fdata;
	if (cmd_node->hd.is_expand)
	{
		expanded = minishell_expand(fdata, env, args);
		minishell_free((void **)&fdata);
	}
	fd = open(cmd_node->hd.filename, O_CREAT | O_RDWR | O_TRUNC);
	if (fd == -1)
		return (false);
	write(fd, expanded, minishell_strlen(expanded));
	minishell_free((void **)&expanded);
	minishell_free((void **)&args.exit);
	return (close(fd), unlink(cmd_node->hd.filename), true);
}

static t_status	handle_ioa(t_root *node, t_root *cmd_node, int32_t input_fd,
		int32_t output_fd)
{
	t_status	status;

	while (minishell_isred(node))
	{
		if (node->ttype == TTOKEN_OUTPUT)
			status = redirect_output(node, output_fd);
		else if (node->ttype == TTOKEN_APPEND)
			status = redirect_append(node, output_fd);
		else if (node->ttype == TTOKEN_INPUT)
			status = redirect_input(node, input_fd);
		else if (node->ttype == TTOKEN_HEREDOC)
			status = redirect_hdoc(cmd_node, input_fd);
		if (status)
			return (STATUS_FAILURE);
		node = node->right;
		while (node && (!minishell_isred(node)))
			node = node->right;
	}
	return (STATUS_SUCCESS);
}

static bool	fd_bkp(int bkpfd[], int32_t input_fd, int32_t output_fd)
{
	bkpfd[0] = dup(input_fd);
	if (bkpfd[0] == -1)
	{
		perror("exec_redirect");
		return (false);
	}
	bkpfd[1] = dup(output_fd);
	if (bkpfd[1] == -1)
	{
		close(bkpfd[0]);
		perror("exec_redirect");
		return (false);
	}
	return (true);
}

static void	fd_undobkp(t_minishell *minishell,
		int32_t fds[], int32_t input_fd, int32_t output_fd)
{
	if (dup2(fds[0], input_fd) == -1)
	{
		perror("dup2");
		minishell_cleanup(minishell, STATUS_FAILURE);
	}
	if (dup2(fds[1], output_fd) == -1)
	{
		perror("dup2");
		minishell_cleanup(minishell, STATUS_FAILURE);
	}
	close(fds[0]);
	close(fds[1]);
}

void	exec_redirect(t_minishell *minishell, t_root *node, int32_t input_fd,
		int32_t output_fd)
{
	t_root	*cmd_node;
	int32_t	bkpfd[2];
	bool	tflag;

	if (!fd_bkp(bkpfd, input_fd, output_fd))
		return ;
	cmd_node = node->left;
	tflag = true;
	if (minishell_translate(node->right, minishell->env,
			minishell_i32tostr(minishell->exit_code))
		|| handle_ioa(node, cmd_node, input_fd, output_fd))
	{
		minishell->exit_code = 1;
		fd_undobkp(minishell, bkpfd, input_fd, output_fd);
		return ;
	}
	if (cmd_node && cmd_node->hd.is_hd)
		tflag = expand_hdoc_in(cmd_node, minishell->env, minishell->exit_code);
	if (tflag)
		exec_cmd(minishell, cmd_node);
	if (cmd_node->hd.is_hd && setbool(&cmd_node->hd.is_hd, false)
		&& minishell_free((void **)&cmd_node->hd.filename))
		close(cmd_node->hd.fd);
	fd_undobkp(minishell, bkpfd, input_fd, output_fd);
}
