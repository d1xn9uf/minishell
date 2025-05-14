/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:47 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:33:47 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

void	minishell_error(t_status status)
{
	if (status != STATUS_EMPTYCMD
		&& status != STATUS_HDOCSIGINT)
		printf("MINISHELL_ERROR : %#08x\n", status);
}
void	minishell_stderr(char *head, char *middle, char *till)
{
	if (head)
		write(STDERR_FILENO, head, minishell_strlen(head));
	if (middle)
		write(STDERR_FILENO, middle, minishell_strlen(middle));
	if (till)
		write(STDERR_FILENO, till, minishell_strlen(till));
}