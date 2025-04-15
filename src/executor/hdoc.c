/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:22 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 17:50:09 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/executor.h"

static char	*insert_newline(char *buffer)
{
	char	*buffer_nl;
	int32_t	i;

	if (!buffer)
		return (NULL);
	buffer_nl = (char *)minishell_calloc(1, minishell_strlen(buffer) + 2);
	i = 0;
	while (buffer[i])
	{
		buffer_nl[i] = buffer[i];
		i++;
	}
	buffer_nl[i] = NEWLINE;
	buffer_nl[++i] = 0;
	minishell_free((void **)&buffer);
	return (buffer_nl);
}

static void	hdoc_input(int32_t fd, char *keyword)
{
	char	*buffer;
	char	*buffer_nl;

	while (true)
	{
		buffer = readline("> ");
		if (!buffer)
		{
			close(fd);
			exit(STATUS_SUCCESS);
		}
		if (minishell_strequal(keyword, buffer))
			break ;
		buffer_nl = insert_newline(buffer);
		write(fd, buffer_nl, minishell_strlen(buffer_nl));
		minishell_free((void **)&buffer_nl);
	}
	minishell_free((void **)&buffer);
	close(fd);
	exit(STATUS_SUCCESS);
}

static t_status	hdoc_keyword_file(t_root *cmd_node, t_root *hdoc_node,
		char **keyword)
{
	if (hdoc_node->right->ttype == TTOKEN_HEREDOC_KEYWORD)
		*keyword = hdoc_node->right->tvalue;
	else
		*keyword = hdoc_node->right->left->tvalue;
	if (minishell_strchr(*keyword, CHAR_SINGLE_QUOTE)
		|| minishell_strchr(*keyword, CHAR_DOUBLE_QUOTE))
	{
		cmd_node->hd.is_expand = false;
		if (hdoc_keyword_noquotes(keyword))
			return (STATUS_HDOCFAILED);
	}
	else
		cmd_node->hd.is_expand = true;
	if (cmd_node->hd.is_hd)
	{
		unlink(cmd_node->hd.filename);
		cmd_node->hd.is_hd = false;
		minishell_free((void **)&cmd_node->hd.filename);
		close(cmd_node->hd.fd);
	}
	cmd_node->hd.filename = minishell_generate_filename();
	cmd_node->hd.fd = open(cmd_node->hd.filename, O_CREAT | O_RDWR, 0644);
	return (STATUS_SUCCESS);
}

static t_status	handle_hdoc(t_root *cmd_node, t_root *hdoc_node)
{
	int32_t	fd;
	int32_t	status;
	char	*keyword;

	status = hdoc_keyword_file(cmd_node, hdoc_node, &keyword);
	if (status || !cmd_node->hd.filename || cmd_node->hd.fd == -1)
		return (STATUS_HDOCFAILED);
	if (fork() == CHILD_PROCESS)
	{
		signal(SIGINT, SIG_DFL);
		fd = open(cmd_node->hd.filename, O_RDWR);
		if (fd == -1)
			exit(STATUS_HDOCFAILED);
		hdoc_input(fd, keyword);
		minishell_free((void **)&keyword);
	}
	if (!cmd_node->hd.is_expand)
		minishell_free((void **)&keyword);
	status = 0;
	waitpid(-1, &status, 0);
	cmd_node->hd.is_hd = true;
	if (WTERMSIG(status) == SIGINT)
		return (STATUS_HDOCSIGINT);
	return (STATUS_SUCCESS);
}

void	executor_handle_hdoc(t_root *root, t_status *status)
{
	t_root	*cmd_node;

	if (!root || *status)
		return ;
	if (minishell_isred(root))
	{
		cmd_node = root->left;
		while (root && minishell_isred(root))
		{
			if (root->ttype == TTOKEN_HEREDOC)
			{
				*status = handle_hdoc(cmd_node, root);
				if (*status)
					return ;
			}
			root = root->right;
		}
	}
	executor_handle_hdoc(root->left, status);
	executor_handle_hdoc(root->right, status);
}
