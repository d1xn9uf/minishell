/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterisk_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:25:02 by mzary             #+#    #+#             */
/*   Updated: 2025/05/13 13:57:55 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static void	order(t_match *names, uint32_t count);

uint32_t	minishell_order(t_match *names)
{
	uint32_t	count;

	count = 0;
	while (names)
	{
		count += 1;
		names = names->next;
	}
	order(names, count);
	return (count);
}

static void	order(t_match *names, uint32_t count)
{
	uint32_t	ord;
	t_match		*s;
	t_match		*c;

	ord = 1;
	while (ord <= count)
	{
		c = names;
		s = c;
		while (c)
		{
			if (!is_anum(c->name) && (is_anum(s->name)
				|| minishell_strcmp(c->name, s->name) < 0))
				s = c;
			else if (is_num(c->name))
			c = c->next;
		}
		s->order = ord;
		ord += 1;
	}
}

