/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:51 by mzary             #+#    #+#             */
/*   Updated: 2025/05/22 13:57:33 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

void	minishell_free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next_token;
		minishell_free((void **)&token->tvalue);
		minishell_free((void **)&token->d.flags);
		minishell_free((void **)&token->a.flags);
		minishell_free((void **)&token);
		token = tmp;
	}
}

void	minishell_free_arr(char **arr)
{
	uint64_t	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
		{
			minishell_free((void **)&arr[i]);
			arr[i] = NULL;
			i++;
		}
		minishell_free((void **)&arr);
		arr = NULL;
	}
}

void	minishell_cleanup(t_minishell *minishell, int32_t exit_status)
{
	minishell_reset(minishell);
	close(minishell->stdfd[0]);
	close(minishell->stdfd[1]);
	minishell_envfree(minishell->env);
	minishell_free((void **)&minishell->cwd);
	minishell_free((void **)&minishell);
	rl_clear_history();
	exit(exit_status);
}

static void	close_hdoc_fds(t_root *root)
{
	if (root)
	{
		close_hdoc_fds(root->left);
		close_hdoc_fds (root->right);
		if (root->hd.is_hd)
			close(root->hd.fd);
	}
}

void	cleanup_fds(t_minishell *minishell, bool closebkp)
{
	if (closebkp)
	{
		close(minishell->stdfd[0]);
		close(minishell->stdfd[1]);
	}
	close_hdoc_fds(minishell->root);
}
