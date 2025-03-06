#include "../../inc/tools.h"

static char	*fetch(char *PATH, char *cmd);
static char	*fetch_dir(char *cmd, char *dir);

char	*minishell_getpath(t_env *env, char *cmd)
{
	char	*PATH;
	t_env	*node;

	if (!env || !cmd)
		return (NULL); // empty environment !
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
		return (NULL); // no PATH in environment
	return (fetch(PATH, cmd));
}

static char	*fetch(char *PATH, char *cmd)
{
	char	**split;
	int		i;
	char	*path;

	split = minishell_split(PATH, ':');
	if (!split)
		return (NULL);
	i = 0;
	path = NULL;
	while (split[i])
	{
		path = fetch_dir(cmd, split[i]);
		if (path)
			break ;
		i += 1;
	}
	return (minishell_free_arr(split), path);
}

static char	*fetch_dir(char *cmd, char *dir)
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
				return (closedir(dirp), NULL);
			path = minishell_strjoin(join, cmd);
			if (!path)
				return (closedir(dirp), free(join), NULL);
			free(join);
		}
		entry = readdir(dirp);
	}
	return (closedir(dirp), path); // if herban leak try to free dirent each loop a jemmi
}
