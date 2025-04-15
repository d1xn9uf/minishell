/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treeutils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:31:19 by mzary             #+#    #+#             */
/*   Updated: 2025/04/15 11:32:01 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

static t_status	protect(t_token *rnode);

t_status	minishell_protect(t_token *root)
{
	t_status	status;

	if (root)
	{
		if (minishell_isred(root) || root->ttype == TTOKEN_PIPE)
		{
			status = protect(root);
			return (status);
		}
		if (minishell_protect(root->right))
			return (STATUS_MALLOCERR);
		if (minishell_protect(root->left))
			return (STATUS_MALLOCERR);
	}
	return (STATUS_SUCCESS);
}

/* handle all pipe cases ??? */
static t_status	protect(t_token *rnode)
{
	if (!rnode->right)
		return (STATUS_SYNTAXERR);
	if (!rnode->left)
	{
		if (rnode->ttype == TTOKEN_PIPE)
			return (STATUS_SYNTAXERR);
		rnode->left = (t_token *)minishell_calloc(1, sizeof(t_token));
		if (!rnode->left)
			return (STATUS_MALLOCERR);
	}
	return (STATUS_SUCCESS);
}
