#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"

# define CHILD_PROCESS 0x00000000
# define PIPE_READ_END 0x00000000
# define PIPE_WRITE_END 0x00000001

# define TMP "/tmp/.00000000000000000000"

// EXECUTION
t_status	minishell_executor(t_minishell *minishell);
void		exec_redirect(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t output_fd);
void		pipeit(t_minishell *minishell, t_root *node, int32_t input_fd);
void		exec_cmd(t_minishell *minishell, t_root *root, int32_t input_fd, int32_t output_fd);

// UTILS
void		executor_handle_hdoc(t_root *root);
char		**executor_getargs(t_root *root);

#endif