/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:48:40 by mzary             #+#    #+#             */
/*   Updated: 2025/05/10 17:51:07 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

bool	setuint32(uint32_t *i, uint32_t v)
{
	if (i)
		*i = v;
	return (true);
}

bool	setchar(char *c, char v)
{
	if (c)
		*c = v;
	return (true);
}

bool	setbool(bool *flag, bool value)
{
	if (flag)
		*flag = value;
	return (true);
}
