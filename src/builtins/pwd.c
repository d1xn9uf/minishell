/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:45 by mzary             #+#    #+#             */
/*   Updated: 2025/05/04 13:56:39 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static char	*get_value_unquote(t_env *l_env, char *key);

t_status	minishell_pwd(t_minishell *minishell, char **argv, t_env *l_env)
{
	char	*cwd;

	if (argv && l_env)
	{
		cwd = pwd(minishell, l_env);
		if (!cwd)
			return (STATUS_MALLOCERR);
		printf("%s\n", cwd);
		minishell_free((void **)&cwd);
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

char	*pwd(t_minishell *minishell, t_env *l_env)
{
	char		*cwd;
	uint32_t	len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = get_value_unquote(l_env, "$PWD");
	if (!cwd)
		return (NULL);
	if (!*cwd)
	{
		len = minishell_strlen(minishell->cwd);
		if (!len)
			return (NULL);
		minishell->cwd[len - 1] = 0;
		cwd = minishell_strdup(minishell->cwd + 1);
		minishell->cwd[len - 1] = CHAR_DOUBLE_QUOTE;
	}
	if (!cwd)
		return (NULL);
	return (cwd);
}

static char	*get_value_unquote(t_env *l_env, char *key)
{
	char		*value;
	uint32_t	len;
	char		*unquoted_value;

	value = minishell_getvalue(l_env, key);
	if (!value)
		return (NULL);
	len = minishell_strlen(value);
	value[len - 1] = 0;
	unquoted_value = minishell_strdup(value + 1);
	value[len - 1] = CHAR_DOUBLE_QUOTE;
	minishell_free((void **)&value);
	return (unquoted_value);
}
