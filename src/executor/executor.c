#include "../../inc/minishell.h"

static void	exec_pipe(t_minishell *minishell, t_root *root)
{
	pipeit(minishell, root, STDIN_FILENO);
}

static void	executor_exec(t_minishell *minishell, t_root *root)
{
	if (root)
	{
		if (root->ttype == TTOKEN_AND_OP)
		{
			executor_exec(minishell, root->left);
			if (!minishell->exit_code)
				executor_exec(minishell, root->right);
		}
		else if (root->ttype == TTOKEN_OR_OP)
		{
			executor_exec(minishell, root->left);
			if (minishell->exit_code)
				executor_exec(minishell, root->right);
		}
		else if (root->ttype == TTOKEN_PIPE)
			exec_pipe(minishell, root);
		else if (minishell_isred(root))
			exec_redirect(minishell, root, STDIN_FILENO, STDOUT_FILENO);
		else if (root->ttype == TTOKEN_COMMAND)
			exec_cmd(minishell, root, STDIN_FILENO ,STDOUT_FILENO);
	}
}

t_status	minishell_executor(t_minishell *minishell)
{
	executor_handle_hdoc(minishell->root);
	executor_exec(minishell, minishell->root);
	return (STATUS_SUCCESS);
}
