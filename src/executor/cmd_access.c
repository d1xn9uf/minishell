/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:47 by mzary             #+#    #+#             */
/*   Updated: 2025/05/15 15:23:26 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/executor.h"

static void	stat_fail(t_minishell *ms, char **argv, t_status *status)
{
	minishell_stderr2("minishell: ", argv[0], ": ", strerror(errno));
	if (errno == ENOTDIR)
		*status = 126;
	else
		*status = 127;
	ms->exit_code = *status;
	minishell_free((void **)&argv);
}

char	**getargs_check(t_minishell *ms, char **argv, t_status *status)
{
	struct stat	st;

	if (!argv[0])
		return (minishell_free((void **)&argv), NULL);
	if (!minishell_isbuiltin(argv[0]))
	{
		if (stat(argv[0], &st))
		{
			stat_fail(ms, argv, status);
			return (NULL);
		}
		errno = 0;
		if (access(argv[0], X_OK) || S_ISDIR(st.st_mode))
		{
			if (!errno)
				errno = EISDIR;
			minishell_stderr2("minishell: ", argv[0], ": ", strerror(errno));
			*status = 126;
			ms->exit_code = *status;
			return (minishell_free((void **)&argv), NULL);
		}
	}
	return (argv);
}
