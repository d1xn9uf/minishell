#include "../../inc/executor.h"

// static void setup_input_output(int32_t input_fd, int32_t output_fd)
// {
//     if (input_fd != 0)
// 	{	
// 		if (dup2(input_fd, STDIN_FILENO) == -1)
// 		{
//             perror("dup2");
//             exit(EXIT_FAILURE);
//         }
//         close(input_fd);
//     }
//     if (output_fd != STDOUT_FILENO)
// 	{
//         if (dup2(output_fd, STDOUT_FILENO) == -1)
// 		{
//             perror("dup2");
//             exit(EXIT_FAILURE);
//         }
//         close(output_fd);
//     }
// }

void	exec_cmd(t_minishell *minishell, t_root *root, int32_t input_fd, int32_t output_fd)
{
	char		**argv;
	pid_t		pid;
	int32_t		status;

	argv = executor_getargs(root);
	//setup_input_output(input_fd, output_fd);
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
	minishell_free_arr(argv);
}
