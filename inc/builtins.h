/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:35:04 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:35:05 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

t_status	minishell_echo(char **argv);

# define CD_ARGS	"minishell_cd: too many arguments\n"
# define CD_HOME	"minishell_cd: HOME not set\n"

t_status	minishell_cd(t_minishell *minishell, char **argv, t_env **l_env);
t_status	minishell_pwd(t_minishell *minishell, char **argv, t_env *l_env);
char		*pwd(t_minishell *minishell, t_env *l_env);

t_status	minishell_export(char **argv, t_env **l_env);
t_status	export(char *key, char *value, t_env **l_env);
t_status	default_export(t_env *env);
t_status	export_inv(char *arg, t_env **l_env);
t_status	minishell_unset(char **argv, t_env **l_env);
t_status	minishell_env(char **argv, t_env *l_env);

t_status	minishell_exit(t_minishell *minishell);

# define ECHOO	"echo"
# define CD		"cd"
# define PWD	"pwd"
# define EXPORT	"export"
# define UNSET	"unset"
# define ENV	"env"
# define EXIT	"exit"

bool		minishell_isbuiltin(char *cmd);
t_status	exec_builtin(t_minishell *minishell, char **argv);

#endif