#include "../../inc/executor.h"

void	exec_cmd(t_root *root, int32_t output_fd, int32_t *exit_code)
{
	char		**argv;
	pid_t		pid;
	int32_t		status;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (output_fd != 1)
			dup2(output_fd, STDOUT_FILENO);
		argv = executor_getargs(root);
		execve(argv[0], argv, NULL);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		*exit_code = 0;
	else
		*exit_code = WEXITSTATUS(status);
}
