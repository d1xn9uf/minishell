#include "../../inc/builtins.h"

t_status	minishell_echo(char **argv, t_env *l_env)
{
	int32_t	i;
	bool	nl;

	if (argv && l_env)
	{
		i = 1;
		nl = true;
		while (argv[i])
		{
			if (i == 1 && minishell_strequal(argv[i], "-n"))
				nl = false;
			else
			{
				if ((i >= 2 && nl) || (i >= 3 && !nl))
					write(STDOUT_FILENO, " ", 1);
				write(STDOUT_FILENO, argv[i], minishell_strlen(argv[i]));
			}
			i += 1;
		}
		if (nl)
			write(STDOUT_FILENO, "\n", 1);
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}
