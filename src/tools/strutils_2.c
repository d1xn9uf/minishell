#include "../../inc/tools.h"

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
			minishell_free_arr(split);
			return (0);
		}
		minishell_strlcpy(split[split_i], s + st, i - st + 1);
		split_i++;
	}
	split[count] = NULL;
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
