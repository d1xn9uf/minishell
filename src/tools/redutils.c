/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:40 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 16:35:56 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

bool	minishell_isred(t_root *node)
{
	if (!node)
		return (false);
	if (node->ttype == TTOKEN_HEREDOC
		|| node->ttype == TTOKEN_OUTPUT
		|| node->ttype == TTOKEN_APPEND
		|| node->ttype == TTOKEN_INPUT)
		return (true);
	return (false);
}

t_status	hdoc_keyword_noquotes(char **keyword)
{
	t_token		interim;
	t_status	status;

	minishell_memset(&interim, 0, sizeof(t_token));
	interim.tvalue = minishell_strdup(*keyword);
	if (!interim.tvalue)
		return (STATUS_MALLOCERR);
	interim.subs = NULL;
	status = minishell_remove(&interim);
	if (status)
		return (status);
	*keyword = interim.tvalue;
	return (STATUS_SUCCESS);
}
