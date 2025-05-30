/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:35:03 by mzary             #+#    #+#             */
/*   Updated: 2025/05/25 18:07:33 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

# define CHILD_PROCESS 0x00000000
# define PIPE_READ_END 0x00000000
# define PIPE_WRITE_END 0x00000001

// EXECUTION
t_status	minishell_executor(t_minishell *minishell);
void		exec_redirect(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t output_fd);
void		pipeit(t_minishell *minishell, t_root *node, int32_t input_fd);
void		exec_cmd(t_minishell *minishell, t_root *root);
void		exec_failed(t_minishell *ms, t_root *cmd_node, int32_t status);

// UTILS
bool		file_node_x_ambig(t_token *red_node, t_token **file_node);
t_status	redirect_output(t_root *node, int32_t output_fd);
t_status	redirect_append(t_root *node, int32_t output_fd);
t_status	redirect_input(t_root *node, int32_t input_fd);
t_status	redirect_hdoc(t_root *cmd_node, int32_t input_fd);
void		executor_handle_hdoc(t_minishell *minishell,
				t_root *root, t_status *status);
char		**executor_getargs(t_root *root, t_minishell *ms, t_status *status);
char		**getargs_check(t_minishell *ms, char **argv, t_status *status);

#endif