#include "../../inc/tools.h"

static t_status	protect(t_token *rnode);

t_status	minishell_protect(t_token *root)
{
	if (root)
	{
		if (minishell_isred(root) && protect(root))
			return (STATUS_MALLOCERR);
		if (minishell_protect(root->right))
			return (STATUS_MALLOCERR);
		if (minishell_protect(root->left))
			return (STATUS_MALLOCERR);
	}
	return (STATUS_SUCCESS);
}

static t_status	protect(t_token *rnode) // m0hc33n: complete this with all cases (QA)
{
	if (!rnode->left)
	{
		rnode->left = (t_token *)minishell_calloc(1, sizeof(t_token));
		if (!rnode->left)
			return (STATUS_MALLOCERR);
	}
	return (STATUS_SUCCESS);
}
