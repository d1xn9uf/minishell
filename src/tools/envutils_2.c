/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envutils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:48 by mzary             #+#    #+#             */
/*   Updated: 2025/05/28 16:28:32 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

static char	*fetch(char *PATH, char *rel_path, t_status *status);
static char	*construct_path(char *dirname, char *rel_path);

char	*minishell_getpath(t_env *env, char *cmd, t_status *status)
{
	t_env	*node;

	if (!cmd || !*cmd)
		return (*status = STATUS_CMDNOTFOUND, NULL);
	if (minishell_strchr(cmd, '/'))
		return (*status = STATUS_SUCCESS, minishell_strdup(cmd));
	node = env;
	while (node)
	{
		if (minishell_strequal("PATH", node->key) && node->valid)
			return (fetch(minishell_strdup(node->value), cmd, status));
		node = node->next_key;
	}
	return (fetch(getcwd(NULL, 0), cmd, status));
}

static char	*fetch(char *env_path, char *rel_path, t_status *status)
{
	char		**split;
	uint32_t	i;
	char		*abs_path;

	if (!env_path)
		return (*status = STATUS_MALLOCERR, NULL);
	split = minishell_split(env_path, ':', NULL);
	if (minishell_free((void **)&env_path) && !split)
		return (*status = STATUS_MALLOCERR, NULL);
	i = 0;
	*status = 0;
	while (split[i])
	{
		abs_path = construct_path(split[i], rel_path);
		if (!abs_path)
			return (minishell_free_arr(split), NULL);
		if (access(abs_path, F_OK) == 0)
			return (minishell_free_arr(split), abs_path);
		minishell_free((void **)&abs_path);
		i += 1;
	}
	*status = STATUS_CMDNOTFOUND;
	return (minishell_free_arr(split), NULL);
}

static char	*construct_path(char *dirname, char *rel_path)
{
	uint32_t	len;

	len = minishell_strlen(dirname);
	if (dirname[len - 1] != '/')
		return (minishell_strjoin3(dirname, "/", rel_path));
	return (minishell_strjoin(dirname, rel_path));
}
