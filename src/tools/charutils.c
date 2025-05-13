/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   charutils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:55:10 by mzary             #+#    #+#             */
/*   Updated: 2025/05/13 13:56:58 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

bool	is_num(char c)
{
	return ('0' <= c && c <= '9');
}

bool	is_up(char c)
{
	return ('A' <= c && c <= 'Z');
}

bool	is_low(char c)
{
	return ('a' <= c && c <= 'z');
}
bool	is_anum(char c)
{
	return (is_num(c) || is_up(c) || is_low(c));
}
