#include "../../inc/executor.h"

static char	*insert_newline(char *buffer)
{
	char	*buffer_nl;
	int32_t	i;

	if (!buffer)
		return (NULL);
	buffer_nl = (char *)malloc(minishell_strlen(buffer) + 2);
	i = 0;
	while (buffer[i])
	{
		buffer_nl[i] = buffer[i];
		i++;
	}
	buffer_nl[i] = NEWLINE;
	buffer_nl[++i] = 0;
	return (buffer_nl);
}

static void	hdoc_input(int32_t fd, char *keyword)
{
	char	*buffer;
	char	*buffer_nl;

	while (true)
	{
		buffer = readline("> ");
		if (minishell_strequal(keyword, buffer))
			break ;
		buffer_nl = insert_newline(buffer);
		write(fd, buffer_nl, minishell_strlen(buffer_nl));
		free(buffer);
		free(buffer_nl);
	}
	close(fd);
}

static void	handle_hdoc(t_root *cmd_node, t_root *hdoc_node)
{
	char	*keyword;
	char	*filename;
	int32_t	fd;

	if (hdoc_node->right->ttype == TTOKEN_HEREDOC_KEYWORD)
		keyword = hdoc_node->right->tvalue;
	else
		keyword = hdoc_node->right->left->tvalue;
	cmd_node->hd.is_hd = true;
	filename = minishell_generate_filename();
	if (filename)
		fd = open(filename, O_CREAT | O_RDWR, 0644);
	else
		exit(EXIT_FAILURE); // TODO : exit cleanly
	hdoc_input(fd, keyword);
	cmd_node->hd.fd = open(filename, O_RDONLY, 0644);
	if (cmd_node->hd.fd == -1)
	{
        perror("handle_hdoc : Failed to create temporary file");
        exit(EXIT_FAILURE); // TODO : exit cleanly
    }
	unlink(filename);
}

void	executor_handle_hdoc(t_root *root)
{
	t_root		*cmd_node;

	if (!root)
		return ;
	if (minishell_isred(root))
	{
		cmd_node = root->left;
		while (root && minishell_isred(root))
		{
			if (root->ttype == TTOKEN_HEREDOC)
				handle_hdoc(cmd_node, root);
			root = root->right;
		}
	}
    executor_handle_hdoc(root->left);
    executor_handle_hdoc(root->right);
}
