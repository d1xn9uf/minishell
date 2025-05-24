/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trans_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:00:14 by mzary             #+#    #+#             */
/*   Updated: 2025/05/24 04:12:58 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

t_status	update_command(t_token *token, t_env *env) // TODO: ROOT CASE
{
	t_status	s;
	char		*temp;

	if (token->is_interpreted && !token->tvalue[0] && token->right)
		token->right->ttype = TTOKEN_COMMAND;
	else
	{
		temp = minishell_getpath(env, token->tvalue, &s);
		if (!temp)
			return (s);
		minishell_free((void **)&token->tvalue);
		token->tvalue = temp;
	}
	return (STATUS_SUCCESS);
}
