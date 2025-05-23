/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:10 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 19:45:03 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static void	fill_flags(char *pattern, t_fixe *fixe, bool *asterisk);
static bool	*convert_to(char *value, bool *ast_flags, uint32_t a_si);

t_fixe	*minishell_analyse(t_token *token)
{
	t_fixe		*fixe;
	char		*pattern;
	bool		*asterisk;

	fixe = (t_fixe *)minishell_calloc(1, sizeof(t_fixe));
	if (!fixe)
		return (NULL);
	pattern = token->tvalue;
	asterisk = convert_to(token->tvalue, token->ast_flags, token->a_si);
	if (!asterisk)
		return (minishell_free((void **)&fixe), NULL);
	fixe->fixes = minishell_split(pattern, '*', asterisk);
	if (!fixe->fixes)
		return (minishell_free((void **)&fixe), NULL);
	fixe->count = 0;
	while (fixe->fixes[fixe->count])
		fixe->count += 1;
	fixe->flags = (t_ast *)minishell_calloc(fixe->count, sizeof(t_ast));
	if (!fixe->flags)
		return (minishell_free((void **)&fixe->fixes),
			minishell_free((void **)&fixe), NULL);
	fill_flags(pattern, fixe, asterisk);
	return (fixe);
}

static bool	*convert_to(char *value, bool *ast_flags, uint32_t a_si)
{
	bool		*asterisk;
	uint32_t	i;

	asterisk = (bool *)minishell_calloc(minishell_strlen(value), sizeof(bool));
	if (!asterisk)
		return (NULL);
	i = 0;
	while (value[i])
	{
		if (value[i] == '*')
		{
			asterisk[i] = ast_flags[a_si];
			a_si += 1;
		}
		i += 1;
	}
	return (asterisk);
}

static void	fill_flags(char *pattern, t_fixe *fixe, bool *asterisk)
{
	uint32_t	i;
	uint32_t	c;
	t_ast		*flag;

	i = 0;
	c = 0;
	while (c < fixe->count)
	{
		flag = fixe->flags + c;
		flag->before = false;
		if (pattern[i] == '*' && asterisk[i])
		{
			flag->before = true;
			while (pattern[i] && pattern[i] == '*' && asterisk[i])
				i += 1;
		}
		while (pattern[i] && !(pattern[i] == '*' && asterisk[i]))
			i += 1;
		flag->after = false;
		if (pattern[i] == '*' && asterisk[i])
			flag->after = true;
		c += 1;
	}
}
