#include "../../inc/tools.h"

static char	*fetch(char *PATH, char *cmd, t_status *status);
static char	*fetch_dir(char *cmd, char *dir, t_status *status);

char	*minishell_getpath(t_env *env, char *cmd, t_status *status)
{
	char	*PATH;
	t_env	*node;

	if (!env || !cmd)
		return (*status = STATUS_CMDNOTFOUND, NULL); // empty environment !
	if (*cmd == '/' || *cmd == '.')
		return (minishell_strdup(cmd));
	PATH = NULL;
	node = env;
	while (node)
	{
		if (minishell_strequal("PATH", node->key))
		{
			PATH = node->value;
			break ;
		}
		node = node->next_key;
	}
	if (!PATH)
		return (*status = STATUS_PATHNOTFOUND, NULL); // no PATH in environment
	return (fetch(PATH, cmd, status));
}

static char	*fetch(char *PATH, char *cmd, t_status *status)
{
	char	**split;
	int		i;
	char	*path;

	split = minishell_split(PATH, ':', NULL);
	if (!split)
		return (*status = STATUS_MALLOCERR, NULL);
	i = 0;
	path = NULL;
	while (split[i])
	{
		path = fetch_dir(cmd, split[i], status);
		if (*status)
			return (minishell_free_arr(split), NULL);
		if (path)
			break ;
		i += 1;
	}
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
			if (!path)
				return (*status = STATUS_MALLOCERR, closedir(dirp), free(join), NULL);
			free(join);
		}
		entry = readdir(dirp);
	}
	return (closedir(dirp), path); // if herban leak try to free dirent each loop a jemmi
}
