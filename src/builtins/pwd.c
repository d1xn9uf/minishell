/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:45 by mzary             #+#    #+#             */
/*   Updated: 2025/05/13 20:42:03 by mzary            ###   ########.fr       */
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
