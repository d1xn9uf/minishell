/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:48:38 by mzary             #+#    #+#             */
/*   Updated: 2025/05/10 16:10:13 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

bool	ignore(t_result *buff, uint32_t i)
{
	uint32_t	j;
	char		q;

	j = 0;
	q = 0;
	while (j < i)
	{
		if (buff->result[j] == CHAR_SINGLE_QUOTE && q != CHAR_DOUBLE_QUOTE)
			q = (q != CHAR_SINGLE_QUOTE) * CHAR_SINGLE_QUOTE;
		if (buff->result[j] == CHAR_DOUBLE_QUOTE && q != CHAR_SINGLE_QUOTE)
			q = (q != CHAR_DOUBLE_QUOTE) * CHAR_DOUBLE_QUOTE;
		j += 1;
	}
	if (buff->result[i + 1] == CHAR_DOUBLE_QUOTE
		|| buff->result[i + 1] == CHAR_SINGLE_QUOTE)
		return (!q);
	return (false);
}
