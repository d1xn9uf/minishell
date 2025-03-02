#include "../../inc/builtins.h"

static void	echo(char *arg);
static void	decide_output(char *q, int *flag);

t_status	minishell_echo(char **argv, t_env *l_env)
{
	int32_t	i;
	int32_t	nl;

	if (argv && l_env)
	{
		i = 1;
		nl = 1;
		while (argv[i])
		{
			if (i == 1 && minishell_strequal(argv[i], "-n"))
				nl = 0;
			else
				echo(argv[i]);
			i += 1;
		}
		if (nl)
			write(STDOUT_FILENO, "\n", 1);
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

static void	echo(char *arg)
{
	int32_t	flag[2];
	int32_t			i;

	i = 0;
	flag[0] = 0;
	flag[1] = 0;
	while (arg[i])
	{
		if (arg[i] == CHAR_SINGLE_QUOTE || arg[i] == CHAR_DOUBLE_QUOTE)
			decide_output(arg + i, flag);
		else
			write(STDOUT_FILENO, arg + i, 1);
		i += 1;
	}
}

static void	decide_output(char *q, int *flag)
{
	if (*q == CHAR_SINGLE_QUOTE)
	{
		if (flag[1])
			write(STDOUT_FILENO, q, 1);
		else
			flag[0]= 1 - flag[0];
	}
	else if (*q == CHAR_DOUBLE_QUOTE)
	{
		if (flag[0])
			write(STDOUT_FILENO, q, 1);
		else
			flag[1] = 1 - flag[1];
	}
}
