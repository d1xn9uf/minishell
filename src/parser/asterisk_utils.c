/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterisk_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:25:02 by mzary             #+#    #+#             */
/*   Updated: 2025/05/13 17:41:14 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static void		order_names(t_match *names, uint32_t count);
static int32_t	comp(char *s1, char *s2);
static void		rearrange_names(t_match **names, uint32_t count);
static t_match	*get_name(t_match *names, uint32_t order);

void	minishell_order(t_match **names)
{
	uint32_t	count;
	t_match		*_names;

	count = 0;
	_names = *names;
	while (_names)
	{
		count += 1;
		_names = _names->next;
	}
	order_names(*names, count);
	rearrange_names(names, count);
}

static void	order_names(t_match *names, uint32_t count)
{
	uint32_t	ord;
	t_match		*smallest;
	t_match		*current;

	ord = 1;
	while (ord <= count)
	{
		current = names;
		while (current->order)
			current = current->next;
		smallest = current;
		while (current)
		{
			if (!current->order && comp(current->name, smallest->name) < 0)
				smallest = current;
			current = current->next;
		}
		smallest->order = ord;
		ord += 1;
	}
}

static int32_t	comp(char *s1, char *s2)
{
	return (minishell_strcmp(s1, s2));
}

static void	rearrange_names(t_match **names, uint32_t count)
{
	uint32_t	ord;
	t_match		*ordered_first;
	t_match		*ordered_names;

	ordered_first = get_name(*names, 1);
	if (!ordered_first)
		return ;
	ordered_names = ordered_first;
	ordered_names->o_next = NULL;
	ord = 2;
	while (ord <= count)
	{
		ordered_names->o_next = get_name(*names, ord);
		if (!ordered_names->o_next)
			return ;
		ordered_names = ordered_names->o_next;
		ordered_names->o_next = NULL;
		ord += 1;
	}
	*names = ordered_first;
}

static t_match	*get_name(t_match *names, uint32_t order)
{
	while (names)
	{
		if (names->order == order)
			return (names);
		names = names->next;
	}
	return (NULL);
}
