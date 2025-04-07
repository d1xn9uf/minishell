#include "../../inc/builtins.h"

t_status	minishell_pwd(t_minishell *minishell, char **argv, t_env *l_env)
{
	char	*cwd;

	if (argv && l_env)
	{
		cwd = pwd(minishell, l_env);
		if (!cwd)
			return (STATUS_MALLOCERR);
		printf("%s\n", cwd);
		free(cwd);
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

char	*pwd(t_minishell *minishell, t_env *l_env)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = minishell_getvalue(l_env, "$PWD");
	if (!cwd)
		return (NULL);
	if (!*cwd)
		cwd = minishell_strdup(minishell->cwd);
	if (!cwd)
		return (NULL);
	return (cwd);
}
