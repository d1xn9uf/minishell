#include "../../inc/builtins.h"

t_status	minishell_pwd(char **argv, t_env *l_env)
{
	char	*cwd;

	if (argv && l_env)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			perror("minishell_pwd: ");
			return (STATUS_FAILURE);
		}
		printf("%s\n", cwd);
		free(cwd);
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}
