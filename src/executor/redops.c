#include "../../inc/executor.h"

void redirect_output(t_root *node, int32_t output_fd)
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

void	redirect_append(t_root *node, int32_t output_fd)
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

void	redirect_input(t_root *node, int32_t input_fd)
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

void	redirect_hdoc(t_root *cmd_node, int32_t input_fd)
{
	if (cmd_node && cmd_node->hd.is_hd)
	{
		if (dup2(cmd_node->hd.fd, input_fd) == -1)
		{
			perror("redirect_hdoc : dup2");
			exit(EXIT_FAILURE);
		}
	}
}
