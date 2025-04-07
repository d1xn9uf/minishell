#include "../../inc/builtins.h"

static t_status	cd(t_minishell *minishell, char *dest, t_env *l_env);
static t_status	update_wd(t_minishell *minishell, char *old_pwd, char *new_pwd, t_env *l_env);

t_status	minishell_cd(t_minishell *minishell, char **argv, t_env *l_env)
{
	char	*dest;

	if (argv && l_env)
	{
		if (argv[1] && argv[2])
			return (write(STDERR_FILENO, CD_ARGS, 33), STATUS_FAILURE);
		if (!argv[1])
			dest = minishell_getvalue(l_env, "$HOME");
		else
			dest = minishell_strdup(argv[1]);
		if (!dest)
				return (STATUS_MALLOCERR);
		if (!argv[1] && !*dest)
			return (free(dest), write(STDERR_FILENO, CD_HOME, 27), STATUS_CMDFAILED);
		return (cd(minishell, dest, l_env));
	}
	return (STATUS_FAILURE);
}

static t_status	cd(t_minishell *minishell, char *dest, t_env *l_env)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = pwd(minishell, l_env);
	if (chdir(dest))
		return (perror("minishell_cd"), free(dest), STATUS_FAILURE);
	free(dest);
	new_pwd = pwd(minishell, l_env);
	if (!old_pwd || !new_pwd)
		return (minishell_free((void **)&old_pwd), minishell_free((void **)&new_pwd), STATUS_FAILURE);
	if (update_wd(minishell, old_pwd, new_pwd, l_env))
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_status	update_wd(t_minishell *minishell, char *old_pwd, char *new_pwd, t_env *l_env)
{
	t_status	status;
	char		*cwd;

	if ((status = export(minishell_strdup("OLDPWD"), old_pwd, l_env)))
		return (free(new_pwd), status);
	if ((status = export(minishell_strdup("PWD"), new_pwd, l_env)))
		return (status);
	cwd = minishell_strdup(new_pwd);
	if (!cwd)
		return (STATUS_MALLOCERR);
	free(minishell->cwd);
	minishell->cwd = cwd;
	return (STATUS_SUCCESS);
}
