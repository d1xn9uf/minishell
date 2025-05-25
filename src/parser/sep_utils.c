/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:00:27 by mzary             #+#    #+#             */
/*   Updated: 2025/05/25 21:03:50 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	replace_value(t_token *token, t_substr *head);

t_status	separate(t_token *token)
{

}

static t_status	replace_value(t_token *token, t_substr *head)
{
	char	*new_value;

	new_value = minishell_concatenate(head);
	if (!new_value)
		return (STATUS_MALLOCERR);
	if (token->tvalue)
		minishell_free((void **)&token->tvalue);
	token->tvalue = new_value;
	token->is_interpreted = true;
	return (STATUS_SUCCESS);
}
