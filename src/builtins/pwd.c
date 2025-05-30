/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:45 by mzary             #+#    #+#             */
/*   Updated: 2025/05/30 13:52:50 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

t_status	minishell_pwd(t_minishell *minishell, char **argv)
{
	char	*cwd;

	if (argv)
	{
		cwd = pwd(minishell);
		if (!cwd)
			return (STATUS_MALLOCERR);
		else if (!*cwd && minishell_free((void **)&cwd))
		{
			minishell_stderr("minishell_pwd: ", "could not get cwd\n", NULL);
			return (STATUS_FAILURE);
		}
		printf("%s\n", cwd);
		minishell_free((void **)&cwd);
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

char	*pwd(t_minishell *minishell)
{
	char		*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = minishell_strdup(minishell->cwd);
	return (cwd);
}
