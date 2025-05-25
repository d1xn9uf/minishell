/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:39 by mzary             #+#    #+#             */
/*   Updated: 2025/05/25 17:35:58 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	reset_token(t_root *root)
{
	if (root)
	{
		reset_token(root->left);
		reset_token(root->right);
		if (root->tvalue)
		{
			minishell_free((void **)&root->tvalue);
		}
		if (root->hd.is_hd)
		{
			if (root->hd.filename)
				unlink(root->hd.filename);
			minishell_free((void **)&root->hd.filename);
			close(root->hd.fd);
		}
		minishell_free((void **)&root);
	}
}

static void	reset_lexer(t_minishell *minishell)
{
	if (minishell)
	{
		if (minishell->lexer->spaced_cmdline)
			minishell_free((void **)&minishell->lexer->spaced_cmdline);
		if (minishell->root)
		{
			reset_token(minishell->root);
			minishell->root = NULL;
		}
		if (minishell->lexer->splited_cmdline)
			minishell_free((void **)&minishell->lexer->splited_cmdline);
		minishell_memset(minishell->lexer, 0, sizeof(t_lexer));
		minishell_free((void **)&minishell->lexer);
	}
}

void	minishell_reset(t_minishell *minishell)
{
	if (minishell)
	{
		if (minishell->lexer)
			reset_lexer(minishell);
		if (minishell->cmdline)
			minishell_free((void **)&minishell->cmdline);
		minishell->pipe_exit = false;
		g_sig_pid = 0;
	}
}
