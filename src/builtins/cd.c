/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:55 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:55 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static t_status	cd(t_minishell *minishell, char *dest, t_env **l_env);
static t_status	update_wd(t_minishell *ms, char *op, char *np, t_env **l_env);

t_status	minishell_cd(t_minishell *minishell, char **argv, t_env **l_env)
{
	char	*dest;

	if (argv && l_env)
	{
		if (argv[1] && argv[2])
			return (write(STDERR_FILENO, CD_ARGS, 33), STATUS_FAILURE);
		if (!argv[1])
			dest = minishell_getvalue(*l_env, "$HOME");
		else
			dest = minishell_strdup(argv[1]);
		if (!dest)
			return (STATUS_MALLOCERR);
		if (!argv[1] && !*dest)
			return (minishell_free((void **)&dest),
				write(STDERR_FILENO, CD_HOME, 27), STATUS_CMDFAILED);
		return (cd(minishell, dest, l_env));
	}
	return (STATUS_FAILURE);
}

static t_status	cd(t_minishell *minishell, char *dest, t_env **l_env)
{
	char	*op;
	char	*np;

	op = pwd(minishell, *l_env);
	if (chdir(dest))
		return (perror("minishell_cd"),
			minishell_free((void **)&dest), STATUS_FAILURE);
	free(dest);
	np = pwd(minishell, *l_env);
	if (!op || !np)
		return (minishell_free((void **)&op),
			minishell_free((void **)&np), STATUS_FAILURE);
	if (update_wd(minishell, op, np, l_env))
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_status	update_wd(t_minishell *ms, char *op, char *np, t_env **l_env)
{
	t_status	status;
	char		*cwd;

	status = export(minishell_strdup("OLDPWD"), op, l_env);
	if (status)
		return (minishell_free((void **)&np), status);
	status = export(minishell_strdup("PWD"), np, l_env);
	if (status)
		return (status);
	cwd = minishell_strdup(np);
	if (!cwd)
		return (STATUS_MALLOCERR);
	minishell_free((void **)&ms->cwd);
	ms->cwd = cwd;
	return (STATUS_SUCCESS);
}
