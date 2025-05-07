/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:37 by mzary             #+#    #+#             */
/*   Updated: 2025/05/07 15:09:01 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

void	minishell_setstatus(t_minishell *minishell, int status)
{
	if (minishell->pipe_exit)
		return ;
	//printf("status is %d\n", status);
	if (WIFEXITED(status))
	{
		minishell->exit_code = WEXITSTATUS(status);
		//printf("[exit0: %d]\n", minishell->exit_code);
	}
	else if (WIFSIGNALED(status))
		minishell->exit_code = 128 + WTERMSIG(status);
	else
	{
		minishell->exit_code = 1;
		//printf("[exit1: %d]\n", minishell->exit_code);
	}
	minishell->pipe_exit = true;
}
