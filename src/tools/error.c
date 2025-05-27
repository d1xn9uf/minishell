/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:47 by mzary             #+#    #+#             */
/*   Updated: 2025/05/15 15:23:26 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

void	minishell_error(t_status status)
{
	if (status == STATUS_EMPTYCMD || status == STATUS_HDOCSIGINT)
		return ;
	else if (status == STATUS_SYNTAXERR)
		minishell_stderr("minishell: SYNTAX_ERROR\n", NULL, NULL);
	else
		printf("MINISHELL_ERROR : %#08x\n", status);
}

bool	minishell_stderr(char *head, char *middle, char *till)
{
	if (head)
		write(STDERR_FILENO, head, minishell_strlen(head));
	if (middle)
		write(STDERR_FILENO, middle, minishell_strlen(middle));
	if (till)
		write(STDERR_FILENO, till, minishell_strlen(till));
	return (true);
}

bool	minishell_stderr2(char *head, char *middle, char *middle2,
			char *middle3, char *till)
{
	if (head)
		write(STDERR_FILENO, head, minishell_strlen(head));
	if (middle)
		write(STDERR_FILENO, middle, minishell_strlen(middle));
	if (middle2)
		write(STDERR_FILENO, middle2, minishell_strlen(middle2));
	if (middle3)
		write(STDERR_FILENO, middle3, minishell_strlen(middle3));
	if (till)
		write(STDERR_FILENO, till, minishell_strlen(till));
	return (true);
}
