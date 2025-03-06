#include "../../inc/builtins.h"

t_status	minishell_cd(char **argv, t_env *l_env) // add cd with no options ~
{
	if (argv && l_env)
	{
		if (!argv[1])
			return (STATUS_SUCCESS);
		if (argv[2])
		{
			write(STDERR_FILENO, "minishell_cd: too many argument\n", 32);
			return (STATUS_FAILURE);
		}
		if (chdir(argv[1]))
		{
			perror("minishell_cd: ");
			return (STATUS_FAILURE);
		}
	}
	return (STATUS_FAILURE);
}
