/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treeutils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:31:19 by mzary             #+#    #+#             */
/*   Updated: 2025/05/22 21:33:49 by mzary            ###   ########.fr       */
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
			if (status)
				return (status);
		}
		status = minishell_protect(root->right);
		if (status)
			return (status);
		status = minishell_protect(root->left);
		if (status)
			return (status);
	}
	return (STATUS_SUCCESS);
}

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

void	fix_tree(t_token *token)
{
	t_token	*rright;

	if (token)
	{
		if (token->right && !token->right->tvalue)
		{
			rright = token->right->right;
			minishell_free((void **)&token->right);
			token->right = rright;
		}
		fix_tree(token->right);
	}
}

void	clean_tree(t_token *token)
{
	if (token)
	{
		if ((token->left && token->left->ttype == TTOKEN_PARENTHESE_CLOSE)
			|| (token->left && token->left->ttype == TTOKEN_PARENTHESE_OPEN))
		{
			minishell_free((void **)&token->left->tvalue);
			minishell_free((void **)&token->left);
			token->left = NULL;
		}
		if ((token->right && token->right->ttype == TTOKEN_PARENTHESE_CLOSE)
			|| (token->right && token->right->ttype == TTOKEN_PARENTHESE_OPEN))
		{
			minishell_free((void **)&token->right->tvalue);
			minishell_free((void **)&token->right);
			token->right = NULL;
		}
		clean_tree(token->left);
		clean_tree(token->right);
	}
}

bool	minishell_freesubs(t_substr *head)
{
	t_substr	*next;
	while (head)
	{
		next = head->next;
		minishell_free((void **)&head->value);
		minishell_free((void **)&head);
		head = next;
	}
	return (true);
}
