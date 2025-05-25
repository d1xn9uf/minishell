/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envutils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:48 by mzary             #+#    #+#             */
/*   Updated: 2025/05/25 20:17:30 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

static char	*fetch(char *PATH, char *cmd, t_status *status, bool free);
static char	*fetch_dir(char *cmd, char *dir, t_status *status);

char	*minishell_getpath(t_env *env, char *cmd, t_status *status)
{
	char	*env_path;
	t_env	*node;
	bool	free;

	if (!env || !cmd)
		return (*status = STATUS_CMDNOTFOUND, NULL);
	if (!*cmd)
		return (*status = STATUS_SUCCESS, NULL);
	if (*cmd == '/' || *cmd == '.')
		return (minishell_strdup(cmd));
	env_path = NULL;
	node = env;
	while (node)
	{
		if (minishell_strequal("PATH", node->key) && node->valid)
		{
			env_path = node->value;
			break ;
		}
		node = node->next_key;
	}
	free = false;
	if (!env_path && setbool(&free, true))
		env_path = getcwd(NULL, 0);
	return (fetch(env_path, cmd, status, free));
}

static char	*fetch(char *env_path, char *cmd, t_status *status, bool free)
{
	char	**split;
	int		i;
	char	*path;

	if (!env_path)
		return (*status = STATUS_MALLOCERR, NULL);
	split = minishell_split(env_path, ':', NULL);
	if ((!free || minishell_free((void **)&env_path)) && !split)
		return (*status = STATUS_MALLOCERR, NULL);
	i = 0;
	path = NULL;
	while (split[i])
	{
		*status = 0;
		path = fetch_dir(cmd, split[i], status);
		if (*status)
			return (minishell_free_arr(split), NULL);
		if (path)
			break ;
		i += 1;
	}
	if (!path)
		*status = STATUS_CMDNOTFOUND;
	return (minishell_free_arr(split), path);
}

static char	*fetch_dir(char *cmd, char *dir, t_status *status)
{
	DIR				*dirp;
	struct dirent	*entry;
	char			*path;
	char			*join;

	dirp = opendir(dir);
	if (!dirp)
		return (NULL);
	entry = readdir(dirp);
	path = NULL;
	while (entry && !path)
	{
		if (minishell_strequal(cmd, entry->d_name))
		{
			join = minishell_strjoin(dir, "/");
			if (!join)
				return (*status = STATUS_MALLOCERR, closedir(dirp), NULL);
			path = minishell_strjoin(join, cmd);
			if (!path && minishell_free((void **)&join))
				return (*status = STATUS_MALLOCERR, closedir(dirp), NULL);
			minishell_free((void **)&join);
		}
		entry = readdir(dirp);
	}
	return (closedir(dirp), path);
}
