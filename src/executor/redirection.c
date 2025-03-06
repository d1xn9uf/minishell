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
        if (dup2(fd, output_fd) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
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
        if (dup2(fd, output_fd) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
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
        if (dup2(fd, input_fd) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
        close(fd);
    }
}

static void	redirect_hdoc(t_root *cmd_node, int32_t input_fd)
{
	if (cmd_node && cmd_node->hd.is_hd)
	{
		if (dup2(cmd_node->hd.fd, input_fd) == -1)
		{
			perror("redirect_hdoc : dup2");
			exit(EXIT_FAILURE);
		}
		//close(cmd_node->hd.fd);
	}
}

static void	handle_ioa(t_root *node, t_root *cmd_node,
				int32_t input_fd, int32_t output_fd)
{
	while (minishell_isred(node))
	{
		if (node->ttype == TTOKEN_OUTPUT)
			redirect_output(node, output_fd);
		else if (node->ttype == TTOKEN_APPEND)
			redirect_append(node, output_fd);
		else if (node->ttype == TTOKEN_INPUT)
			redirect_input(node, input_fd);
		else if (node->ttype == TTOKEN_HEREDOC)
			redirect_hdoc(cmd_node, input_fd);
		node = node->right;
		while (node && (!minishell_isred(node)))
			node = node->right;
	}
}

void		exec_redirect(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t output_fd)
{
	t_root	*cmd_node;
	int32_t	bkpfd[2];

	bkpfd[0] = dup(input_fd);
	bkpfd[1] = dup(output_fd);
	cmd_node = node->left;
	handle_ioa(node, cmd_node, input_fd, output_fd);
	exec_cmd(minishell, cmd_node, input_fd, output_fd);
	dup2(bkpfd[0], input_fd);
	dup2(bkpfd[1], output_fd);
}
