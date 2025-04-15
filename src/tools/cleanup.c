/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:51 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:33:52 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

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
