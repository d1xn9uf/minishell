#include "../../inc/builtins.h"

bool		minishell_isbuiltin(char *cmd)
{
	if (minishell_strequal(cmd, EXPORT))
		return (true);
	else if (minishell_strequal(cmd, UNSET))
		return (true);
	else if (minishell_strequal(cmd, ECHO))
		return (true);
	else if (minishell_strequal(cmd, EXIT))
		return (true);
	else if (minishell_strequal(cmd, ENV))
		return (true);
	else if (minishell_strequal(cmd, PWD))
		return (true);
	else if (minishell_strequal(cmd, CD))
		return (true);
	return (false);
}

t_status	exec_builtin(t_minishell *minishell, char **argv)
{
	t_status	status;

	if (minishell_strequal(argv[0], EXPORT))
		status = minishell_export(argv, minishell->env);
	else if (minishell_strequal(argv[0], UNSET))
		status = minishell_unset(argv, &minishell->env);
	else if (minishell_strequal(argv[0], ECHO))
		status = minishell_echo(argv, minishell->env);
	else if (minishell_strequal(argv[0], ENV))
		status = minishell_env(argv, minishell->env);
	else if (minishell_strequal(argv[0], PWD))
		status = minishell_pwd(minishell, argv, minishell->env);
	else if (minishell_strequal(argv[0], CD))
		status = minishell_cd(minishell, argv, minishell->env);
	else if (minishell_strequal(argv[0], EXIT))
		status = minishell_exit(minishell);
	return (status);
}
