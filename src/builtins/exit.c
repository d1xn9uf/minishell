/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:49 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:34:50 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

t_status	minishell_exit(t_minishell *minishell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	minishell_cleanup(minishell, STATUS_SUCCESS);
	return (STATUS_SUCCESS);
}
