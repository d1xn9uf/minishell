#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

t_status	minishell_echo(char **argv, t_env *l_env);

t_status	minishell_cd(t_minishell *minishell, char **argv, t_env *l_env);
# define	CD_ARGS	"minishell_cd: too many arguments\n"
# define	CD_HOME	"minishell_cd: HOME not set\n"
t_status	minishell_pwd(t_minishell *minishell, char **argv, t_env *l_env);
char		*pwd(t_minishell *minishell, t_env *l_env);

t_status	minishell_export(char **argv, t_env *l_env);
t_status	export(char *key, char *value, t_env *l_env);
t_status	default_export(t_env *env);
t_status	minishell_unset(char **argv, t_env **l_env);
t_status	minishell_env(char **argv, t_env *l_env);

t_status	minishell_exit(t_minishell *minishell);


# define ECHO	"echo"		//ok
# define CD		"cd"		//ok
# define PWD	"pwd"		//ok
# define EXPORT	"export"	//
# define UNSET	"unset"		//ok
# define ENV	"env"		//ok
# define EXIT	"exit"		//ok

bool		minishell_isbuiltin(char *cmd);
t_status	exec_builtin(t_minishell *minishell, char **argv);

#endif