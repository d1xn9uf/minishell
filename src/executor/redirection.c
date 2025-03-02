#include "../../inc/executor.h"

static void redirect_output(t_root *node, int32_t output_fd)
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
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, output_fd);
        close(fd);
    }
}

static void redirect_append(t_root *node, int32_t output_fd)
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
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, output_fd);
        close(fd);
    }
}

static void redirect_input(t_root *node, int32_t input_fd)
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
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, input_fd);
        close(fd);
    }
}

static void redirect_heredoc(t_root *node)
{
    int32_t	pipe_fd[2];
    pid_t	pid;
    
    pipe(pipe_fd);
    pid = fork();
    if (pid == 0)
    {
        close(pipe_fd[0]);
        // Write the here document input to the pipe
        write(pipe_fd[1], node->right->tvalue, strlen(node->right->tvalue));
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        
        // Execute the command with redirected input
        execve(node->left->tvalue, executor_getargs(node->left), NULL);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        wait(NULL);
    }
}

void		exec_redirect(t_root *node, int32_t input_fd,
				int32_t output_fd, int32_t *exit_code)
{
	t_root	*cmd_node;
	int32_t	bkpfd[2];

	bkpfd[0] = dup(input_fd);
	bkpfd[1] = dup(output_fd);
	cmd_node = node->left;
	while (minishell_isred(node))
	{
		if (node->ttype == TTOKEN_OUTPUT)
			redirect_output(node, output_fd);
		else if (node->ttype == TTOKEN_APPEND)
			redirect_append(node, output_fd);
		else if (node->ttype == TTOKEN_INPUT)
			redirect_input(node, input_fd);
		else if (node->ttype == TTOKEN_HEREDOC)
			redirect_heredoc(node);
		node = node->right;
		while (node && (!minishell_isred(node)))
			node = node->right;
	}
	exec_cmd(cmd_node, output_fd, exit_code);
	dup2(bkpfd[1], output_fd);
	dup2(bkpfd[0], input_fd);
}
