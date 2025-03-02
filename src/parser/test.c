#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*---------------------------------------------------------------*/
size_t	minishell_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*(s + len))
		len++;
	return (len);
}

size_t	minishell_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;
	char	*s;

	i = 0;
	s = (char *)src;
	src_len = minishell_strlen(src);
	while (dstsize != 0 && i < dstsize - 1 && *(s + i))
	{
		*(dst + i) = *(s + i);
		i++;
	}
	if (dstsize != 0)
		*(dst + i) = '\0';
	return (src_len);
}
/*---------------------------------------------------------------*/
static int	s_count(char const *s, char c);
static int	ft_split(char const *s, char c, char **split, int count);

char		**minishell_split(char *s, char c)
{
	char	**split;
	int		count;

	if (!s)
		return (NULL);
	count = s_count(s, c);
	split = (char **)malloc(sizeof(char *) * (count + 1));
	if (split == NULL)
		return (NULL);
	if (!ft_split(s, c, split, count))
		return (NULL);
	return (split);
}

static int	s_count(char const *s, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static int	ft_split(char const *s, char c, char **split, int count)
{
	int	i;
	int	st;
	int	split_i;

	i = 0;
	split_i = 0;
	while (split_i < count)
	{
		while (s[i] && s[i] == c)
			i++;
		st = i;
		while (s[i] && s[i] != c)
			i++;
		split[split_i] = (char *)malloc(sizeof(char) * (i - st + 1));
		if (split[split_i] == NULL)
		{
			return (0);
		}
		minishell_strlcpy(split[split_i], s + st, i - st + 1);
		split_i++;
	}
	split[count] = NULL;
	return (1);
}
/*---------------------------------------------------------------*/
static int			found_string(char *string, char *s)
{
	size_t	i;

	i = 0;
	while (string[i] && s[i] && string[i] == s[i])
		i += 1;
	if (!string[i])
		return (1);
	return (0);
}

static size_t		contains_string(char *string, char *s)
{
	size_t	start_i;

	start_i = 0;
	while (s[start_i])
	{
		if (found_string(string, s + start_i))
			return (start_i);
		start_i += 1;
	}
	return (start_i);
}
/*---------------------------------------------------------------*/
typedef struct s_ast
{
	int	before;
	int	after;
}			t_ast;

typedef struct s_fixe
{
	char		**fixes;
	t_ast	*flags;
	size_t	count;
}			t_fixe;

static void	fill_flags(char *pattern, t_fixe *fixe)
{
	size_t	i;
	size_t	c;
	t_ast 	*flag;

	i = 0;
	c = 0;
	while (c < fixe->count)
	{
		flag = fixe->flags + c;
		if (pattern[i] == '*')
		{
			flag->before = 1;
			while (pattern[i] && pattern[i] == '*')
				i += 1;
		}
		while (pattern[i] && pattern[i] != '*')
			i += 1;
		if (pattern[i] == '*')
			flag->after = 1;
		c += 1;
	}
}

t_fixe	*split_pattern(char *pattern)
{
	t_fixe	*fixe;
	char	**fixes;
	int *flags;
	size_t	i;

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
/*----------------------------------------------------------------*/
static int	matches_pattern(char *pattern, t_fixe *fixe, char *s)
{
	size_t	c;
	size_t	start_i;
	
	c = 0;
	start_i = 0;
	while (c < fixe->count)
	{
		start_i = contains_string(fixe->fixes[c], s);
		if (!s[start_i])
			return (0);
		if (c == 0 && !fixe->flags[c].before && start_i != 0)
			return (0);
		start_i += minishell_strlen(fixe->fixes[c]);
		if (c == fixe->count - 1 && !fixe->flags[c].after && s[start_i])
			return (0);
		c += 1;
	}
	return (1);
}

int main(int ac, char **av)
{
	if (matches_pattern(av[1], split_pattern(av[1]), av[2]))
		printf("MATCHES!\n");
	return (0);
}

