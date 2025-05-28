/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getargs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:23 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 16:13:58 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/executor.h"

static uint32_t	getargs_argc(t_root *root)
{
	uint32_t	sz;

	sz = 0;
	if (root)
	{
		while (root)
		{
			sz++;
			root = root->right;
		}
		return (sz);
	}
	return (0);
}

static char	**getargs_init(t_root *root, uint32_t *argc)
{
	char	**argv;

	if (root)
	{
		*argc = getargs_argc(root);
		argv = (char **)minishell_calloc(1, sizeof(char *) * (*argc + 1));
		if (!argv)
			return (NULL);
		return (argv);
	}
	return (NULL);
}

static void	getargs_setargc(t_root *root, char **argv)
{
	uint32_t	count;

	count = 0;
	while (root)
	{
		if (root->tvalue && (root->tvalue[0] || !root->is_interpreted))
			argv[count++] = root->tvalue;
		root = root->right;
	}
	argv[count] = 0;
}

char	**executor_getargs(t_root *root, t_minishell *ms, t_status *status)
{
	char		**argv;
	uint32_t	argc;

	*status = minishell_translate(root, ms->env,
			minishell_i32tostr(ms->exit_code));
	if (*status)
	{
		if (*status != STATUS_MALLOCERR)
			ms->exit_code = 127;
		return (NULL);
	}
	argv = getargs_init(root, &argc);
	if (!argv || !argc)
		return (NULL);
	getargs_setargc(root, argv);
	return (getargs_check(ms, argv, status));
}
