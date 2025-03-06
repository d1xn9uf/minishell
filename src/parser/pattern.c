#include "../../inc/parser.h"

static void	fill_flags(char *pattern, t_fixe *fixe);

t_fixe	*split_pattern(char *pattern)
{
	t_fixe		*fixe;

	fixe = (t_fixe *)malloc(sizeof(t_fixe));
	if (!fixe)
		return (NULL);
	fixe->fixes = minishell_split(pattern, '*');
	if (!fixe->fixes)
		return (free(fixe), NULL);
	fixe->count = 0;
	while (fixe->fixes[fixe->count])
		fixe->count += 1;
	fixe->flags = (t_ast *)malloc(sizeof(t_ast) * fixe->count);
	if (!fixe->flags)
		return (free(fixe->fixes), free(fixe), NULL);
	fill_flags(pattern, fixe);
	return (fixe);
}

static void	fill_flags(char *pattern, t_fixe *fixe)
{
	uint32_t	i;
	uint32_t	c;
	t_ast 		*flag;

	i = 0;
	c = 0;
	while (c < fixe->count)
	{
		flag = fixe->flags + c;
		flag->before = false;
		if (pattern[i] == '*')
		{
			flag->before = true;
			while (pattern[i] && pattern[i] == '*')
				i += 1;
		}
		while (pattern[i] && pattern[i] != '*')
			i += 1;
		flag->after = false;
		if (pattern[i] == '*')
			flag->after = true;
		c += 1;
	}
}
/*----------------------------------------------------------------------------*/

static uint32_t	contains_string(char *string, char *s);
static bool		found_string(char *string, char *s);

bool	matches_pattern(t_fixe *fixe, char *s)
{
	uint32_t	c;
	uint32_t	min_start;
	uint32_t	start_i;
	
	c = 0;
	min_start = 0;
	start_i = 0;
	while (c < fixe->count)
	{
		start_i = contains_string(fixe->fixes[c], s);
		if (!s[start_i] || start_i < min_start)
			return (false);
		if (c == 0 && !fixe->flags[c].before && start_i != 0)
			return (false);
		start_i += minishell_strlen(fixe->fixes[c]);
		min_start = start_i;
		if (c == fixe->count - 1 && !fixe->flags[c].after && s[start_i])
			return (false);
		c += 1;
	}
	return (true);
}

static uint32_t	contains_string(char *string, char *s)
{
	uint32_t	start_i;

	start_i = 0;
	while (s[start_i])
	{
		if (found_string(string, s + start_i))
			return (start_i);
		start_i += 1;
	}
	return (start_i);
}

static bool		found_string(char *string, char *s)
{
	uint32_t	i;

	i = 0;
	while (string[i] && s[i] && string[i] == s[i])
		i += 1;
	if (!string[i])
		return (true);
	return (false);
}
