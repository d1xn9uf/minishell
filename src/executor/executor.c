#include "../../inc/minishell.h"

static void	executor_exec(t_root *root, int32_t *exit_code)
{
	if (root)
	{
		if (root->ttype == TTOKEN_AND_OP)
		{
			executor_exec(root->left, exit_code);
			if (!*exit_code)
				executor_exec(root->right, exit_code);
		}
		else if (root->ttype == TTOKEN_OR_OP)
		{
			executor_exec(root->left, exit_code);
			if (*exit_code)
				executor_exec(root->right, exit_code);
		}
		else if (root->ttype == TTOKEN_PIPE)
			exec_pipe(root, exit_code);
		else if (minishell_isred(root))
			exec_redirect(root, 0, 1, exit_code);
		else if (root->ttype == TTOKEN_COMMAND)
			exec_cmd(root, STDOUT_FILENO, exit_code);
	}
}

t_status	minishell_executor(t_minishell *minishell)
{
	executor_exec(minishell->root, &minishell->exit_code);
	return (STATUS_SUCCESS);
}
