#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"

# define CHILD_PROCESS 0x00000000
# define PIPE_READ_END 0x00000000
# define PIPE_WRITE_END 0x00000001

// EXECUTION
t_status	minishell_executor(t_minishell *minishell);
void		exec_redirect(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t output_fd);
void		pipeit(t_minishell *minishell, t_root *node, int32_t input_fd);
void		exec_cmd(t_minishell *minishell, t_root *root);

// UTILS
void		redirect_output(t_root *node, int32_t output_fd);
void		redirect_append(t_root *node, int32_t output_fd);
void		redirect_input(t_root *node, int32_t input_fd);
void		redirect_hdoc(t_root *cmd_node, int32_t input_fd);
void		executor_handle_hdoc(t_root *root, t_status *status);
char		**executor_getargs(t_root *root, t_minishell *ms, t_status *status);

#endif