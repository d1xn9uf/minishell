#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"

# define CHILD_PROCESS 0x00000000
# define PIPE_READ_END 0x00000000
# define PIPE_WRITE_END 0x00000001

// EXECUTION
t_status	minishell_executor(t_minishell *minishell);
void		exec_redirect(t_root *node, int32_t input_fd,
				int32_t output_fd, int32_t *exit_code);
void		exec_pipe(t_root *root, int32_t *exit_code);
void		exec_cmd(t_root *root, int32_t output_fd, int32_t *exit_code);

// UTILS
char		**executor_getargs(t_root *root);

#endif