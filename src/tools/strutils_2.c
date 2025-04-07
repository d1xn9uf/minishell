#include "../../inc/tools.h"

typedef struct s_split
{
	char	*s;
	bool	*flags;
	char	c;
	int		count;
	char	**split;
}	t_split;

static int	s_count(t_split *split);
static int	ft_split(t_split *split);

char		**minishell_split(char *s, char c, bool *flags)
{
	t_split	split;

	split.s = s;
	if (!split.s)
		return (NULL);
	split.flags = flags;
	split.c = c;
	split.count = s_count(&split);
	split.split = (char **)malloc(sizeof(char *) * (split.count + 1));
	if (split.split == NULL)
		return (NULL);
	if (!ft_split(&split))
		return (NULL);
	return (split.split);
}

static int	s_count(t_split *split)
{
	int			i;

	i = 0;
	split->count = 0;
	while (split->s[i])
	{
		if (split->s[i] == split->c && (!split->flags || split->flags[i]))
			i++;
		else
		{
			split->count++;
			while (split->s[i] && (split->s[i] != split->c || (split->flags && !split->flags[i])))
				i++;
		}
	}
	return (split->count);
}

static int	ft_split(t_split *split)
{
	int	i;
	int	st;
	int	split_i;

	i = 0;
	split_i = 0;
	while (split_i < split->count)
	{
		while (split->s[i] && split->s[i] == split->c 
				&& (!split->flags || split->flags[i]))
			i++;
		st = i;
		while (split->s[i] && (split->s[i] != split->c || (split->flags && !split->flags[i])))
			i++;
		split->split[split_i] = (char *)malloc(sizeof(char) * (i - st + 1));
		if (split->split[split_i] == NULL)
		{
			minishell_free_arr(split->split);
			return (0);
		}
		minishell_strlcpy(split->split[split_i], split->s + st, i - st + 1);
		split_i++;
	}
	split->split[split->count] = NULL;
	return (1);
}

size_t		minishell_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len;

	if (!dst && !size)
		return (minishell_strlen(src)); // thank you Imane
	len = minishell_strlen((const char *)dst);
	if (size >= minishell_strlen((const char *)dst) + 1)
	{
		minishell_strlcpy(dst + minishell_strlen(dst), src, size - minishell_strlen(dst));
		return (len + minishell_strlen(src));
	}
	else
		return (size + minishell_strlen(src));
}
