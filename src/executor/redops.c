/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redops.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:20 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:20 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/executor.h"

t_status	redirect_output(t_root *node, int32_t output_fd)
{
	int32_t	fd;
	t_root	*file_node;

	file_node = NULL;
	if (minishell_isred(node->right))
		file_node = node->right->left;
	else
		file_node = node->right;
	if (file_node && file_node->ttype == TTOKEN_FILE)
	{
		fd = open(file_node->tvalue, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("minishell");
			return (STATUS_FAILURE);
		}
		if (dup2(fd, output_fd) == -1)
		{
			perror("dup2");
			return (STATUS_FAILURE);
		}
		close(fd);
	}
	return (STATUS_SUCCESS);
}

t_status	redirect_append(t_root *node, int32_t output_fd)
{
	int32_t	fd;
	t_root	*file_node;

	file_node = NULL;
	if (minishell_isred(node->right))
		file_node = node->right->left;
	else
		file_node = node->right;
	if (file_node && file_node->ttype == TTOKEN_FILE)
	{
		fd = open(file_node->tvalue, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("minishell");
			return (STATUS_FAILURE);
		}
		if (dup2(fd, output_fd) == -1)
		{
			perror("dup2");
			return (STATUS_FAILURE);
		}
		close(fd);
	}
	return (STATUS_SUCCESS);
}

t_status	redirect_input(t_root *node, int32_t input_fd)
{
	int32_t	fd;
	t_root	*file_node;

	file_node = NULL;
	if (minishell_isred(node->right))
		file_node = node->right->left;
	else
		file_node = node->right;
	if (file_node && file_node->ttype == TTOKEN_FILE)
	{
		fd = open(file_node->tvalue, O_RDONLY);
		if (fd == -1)
		{
			perror("minishell");
			return (STATUS_FAILURE);
		}
		if (dup2(fd, input_fd) == -1)
		{
			perror("dup2");
			return (STATUS_FAILURE);
		}
		close(fd);
	}
	return (STATUS_SUCCESS);
}

t_status	redirect_hdoc(t_root *cmd_node, int32_t input_fd)
{
	if (cmd_node && cmd_node->hd.is_hd)
	{
		if (dup2(cmd_node->hd.fd, input_fd) == -1)
		{
			perror("redirect_hdoc : dup2");
			return (STATUS_FAILURE);
		}
	}
	return (STATUS_SUCCESS);
}
