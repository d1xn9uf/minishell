#include "../../inc/executor.h"

void	exec_cmd(t_minishell *minishell, t_root *cmd_node)
{
	char		**argv;
	pid_t		pid;
	int32_t		status;

	status = 0;
	argv = executor_getargs(cmd_node, minishell, (t_status *)&status);
	if (!argv)
		return ;
	if (minishell_isbuiltin(argv[0]))
		minishell->exit_code = exec_builtin(minishell, argv);
	else
	{
		pid = fork();
		if (pid == CHILD_PROCESS)
		{
			execve(argv[0], argv, NULL);
			exit(EXIT_FAILURE);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			minishell->exit_code = 0;
		else
			minishell->exit_code = WEXITSTATUS(status);
	}
	free(argv);
}
