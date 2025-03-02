#include "../../inc/executor.h"

static void pipeit_child(t_root *node, int32_t input_fd,
	int32_t output_fd, int32_t *exit_code)
{
    char **argv;

    if (minishell_isred(node->left))
        exec_redirect(node->left, input_fd, output_fd, exit_code);
    if (input_fd != 0)
    {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (node->ttype == TTOKEN_PIPE)
    {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
        argv = executor_getargs(node->left);
    }
    else
        argv = executor_getargs(node);
    execve(argv[0], argv, NULL);
    exit(EXIT_FAILURE);
}

static void	pipeit(t_root *node, int32_t input_fd, int32_t *exit_code)
{
	int32_t pipe_fd[2];
	pid_t	pid;
	int32_t	status;	

	if (node == NULL)
		return ;
	if (node->ttype == TTOKEN_PIPE)
		pipe(pipe_fd);
	pid = fork();
	if (pid == CHILD_PROCESS)
		pipeit_child(node, input_fd, pipe_fd[PIPE_WRITE_END], exit_code);
	else if (pid > 0)
	{
		if (input_fd != 0)
			close(input_fd); // Close the old input
		if (node->ttype == TTOKEN_PIPE)
			close(pipe_fd[PIPE_WRITE_END]); // Close the write end of the pipe
		if (node->ttype == TTOKEN_PIPE)
			pipeit(node->right, pipe_fd[PIPE_READ_END], exit_code);
		else
			pipeit(node->right, input_fd, exit_code);
		waitpid(pid, &status, 0);
		if (WIFEXITED(*exit_code) && WEXITSTATUS(status) != 0)
			*exit_code = WEXITSTATUS(status);
    }
}

void	exec_pipe(t_root *root, int32_t *exit_code)
{
	if (root)
	{
		pipeit(root, 0, exit_code);
	}
}
