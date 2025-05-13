/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:55 by mzary             #+#    #+#             */
/*   Updated: 2025/05/13 18:11:34 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static char		*get_value_unquote(t_env *l_env, char *key);
static char		*get_path(char *arg, t_env *l_env);
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
		{
			dest = get_value_unquote(*l_env, "$HOME");
			if (dest && !*dest)
				return (minishell_free((void **)&dest),
					write(STDERR_FILENO, CD_HOME, 27), STATUS_FAILURE);
		}
		else
			dest = get_path(argv[1], *l_env);
		if (!dest)
			return (STATUS_MALLOCERR);
		if (!argv[1] && !*dest)
			return (minishell_free((void **)&dest),
				write(STDERR_FILENO, CD_HOME, 27), STATUS_CMDFAILED);
		return (cd(minishell, dest, l_env));
	}
	return (STATUS_FAILURE);
}

static char	*get_value_unquote(t_env *l_env, char *key)
{
	char		*value;
	uint32_t	len;
	char		*unquoted_value;

	value = minishell_getvalue(l_env, key);
	if (!value)
		return (NULL);
	len = minishell_strlen(value);
	value[len - 1] = 0;
	unquoted_value = minishell_strdup(value + 1);
	value[len - 1] = CHAR_DOUBLE_QUOTE;
	minishell_free((void **)&value);
	return (unquoted_value);
}

static char	*get_path(char *arg, t_env *l_env)
{
	char	*home_path;
	char	*path;

	if (arg[0] == '~' && (!arg[1] || arg[1] == '/'))
	{
		home_path = get_value_unquote(l_env, "$HOME");
		if (!home_path)
			return (NULL);
		path = minishell_strjoin(home_path, arg + 1);
		minishell_free((void **)&home_path);
		return (path);
	}
	return (minishell_strdup(arg));
}

static t_status	cd(t_minishell *minishell, char *dest, t_env **l_env)
{
	char	*op;
	char	*np;

	op = pwd(minishell, *l_env);
	if (chdir(dest))
		return (perror("minishell_cd"),
			minishell_free((void **)&dest),
			minishell_free((void **)&op), STATUS_FAILURE);
	minishell_free((void **)&dest);
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
