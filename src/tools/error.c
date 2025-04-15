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
