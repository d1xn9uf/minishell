/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:52 by mzary             #+#    #+#             */
/*   Updated: 2025/05/11 19:17:04 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static bool	is_newline(char *arg);

t_status	minishell_echo(char **argv)
{
	int32_t	i;
	bool	nl;

	if (argv)
	{
		i = 1;
		nl = true;
		while (is_newline(argv[i]) && setbool(&nl, false))
			i += 1;
		while (argv[i])
		{
			printf("%s", argv[i]);
			i += 1;
			if (argv[i])
				printf(" ");
		}
		if (nl)
			printf("\n");
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

static bool	is_newline(char *arg)
{
	uint32_t	i;

	if (arg && arg[0] == '-')
	{
		i = 1;
		while (arg[i] == 'n')
			i += 1;
		return (!arg[i]);
	}
	return (false);
}
