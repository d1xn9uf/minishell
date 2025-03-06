#include "../../inc/executor.h"

static void exec_redirect_if_needed(t_minishell *minishell, t_root *node,
                                    int32_t input_fd, int32_t output_fd)
{
    t_root *rnode = NULL;
    
    if (minishell_isred(node) || minishell_isred(node->left))
	{
		if (minishell_isred(node))
			rnode = node;
		else
			rnode = node->left;
        exec_redirect(minishell, rnode, input_fd, output_fd);
        exit(minishell->exit_code);
    }
}

static void setup_input_output(t_root *node, int32_t input_fd, int32_t output_fd)
{
    if (input_fd != 0 || node->hd.is_hd)
	{
		if (node->hd.is_hd && dup2(node->hd.fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
        else if (dup2(input_fd, STDIN_FILENO) == -1)
		{
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(input_fd);
    }
    if (output_fd != STDOUT_FILENO)
	{
        if (dup2(output_fd, STDOUT_FILENO) == -1)
		{
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(output_fd);
    }
}

static void pipeit_child(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t output_fd)
{
    char	**argv;
	t_root	*cmd_node;

	exec_redirect_if_needed(minishell, node, input_fd, output_fd);
	if (node->ttype == TTOKEN_COMMAND)
		cmd_node = node;
	else
		cmd_node = node->left;
	setup_input_output(cmd_node, input_fd, output_fd);
	if (node->ttype == TTOKEN_PIPE)
        argv = executor_getargs(node->left);
    else
        argv = executor_getargs(node);
	if (minishell_isbuiltin(argv[0]))
		exec_builtin(minishell, argv);
	else
		execve(argv[0], argv, NULL);
    exit(EXIT_FAILURE);
}

static void handle_parent(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t pipe_fd[2], pid_t pid)
{
    int32_t status;

    if (input_fd != 0)
        close(input_fd);
    if (node->ttype == TTOKEN_PIPE)
        close(pipe_fd[PIPE_WRITE_END]);
    if (node->ttype == TTOKEN_PIPE)
        pipeit(minishell, node->right, pipe_fd[PIPE_READ_END]);
    
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        minishell->exit_code = WEXITSTATUS(status);
}

void	pipeit(t_minishell *minishell, t_root *node, int32_t input_fd)
{
	int32_t pipe_fd[2];
	pid_t	pid;
	int32_t	status;	

	if (node == NULL)
		return ;
	if (node->ttype == TTOKEN_PIPE && pipe(pipe_fd) == -1) 
	{
        perror("pipe");
        exit(EXIT_FAILURE); // TODO exit cleanly
    }
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
        if (node->ttype == TTOKEN_PIPE)
            pipeit_child(minishell, node, input_fd, pipe_fd[PIPE_WRITE_END]);
        else
        	pipeit_child(minishell, node, input_fd, minishell->stdfd[1]);
    }	
	else if (pid > 0)
		handle_parent(minishell, node, input_fd, pipe_fd, pid);
}
