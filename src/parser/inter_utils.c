/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:47:34 by mzary             #+#    #+#             */
/*   Updated: 2025/05/26 09:49:23 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

void	minishell_ambig(t_token *token, uint32_t c)
{
	t_token	*remove;

	if (token->ambig.red_flag
		&& (c > 1 || (!*token->tvalue && !token->is_empty)))
	{
		token->ambig.is_ambiguous = true;
		minishell_free((void **)&token->tvalue);
		token->tvalue = token->ambig.saver;
		while (token->right && token->right->is_interpreted)
		{
			remove = token->right;
			token->right = token->right->right;
			remove->right = NULL;
			remove->left = NULL;
			reset_token(remove);
		}
	}
}
