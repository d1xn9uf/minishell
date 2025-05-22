/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:37 by mzary             #+#    #+#             */
/*   Updated: 2025/05/15 14:35:24 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

void	minishell_setstatus(t_minishell *minishell, int status)
{
	if (WIFEXITED(status))
	{
		minishell->exit_code = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
		minishell->exit_code = 128 + WTERMSIG(status);
	else
		minishell->exit_code = 1;
}
