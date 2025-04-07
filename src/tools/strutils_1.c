#include "../../inc/tools.h"

int32_t	minishell_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char	*minishell_strdup(const char *s)
{
	char	*c;

	if (!s)
		return (NULL);
	c = (char *)malloc(sizeof(char) * (minishell_strlen(s) + 1));
	if (c == NULL)
		return (NULL);
	minishell_strlcpy(c, s, minishell_strlen(s) + 1);
	return (c);
}

bool	minishell_strequal(const char *s1, const char *s2)
{
	if (minishell_strlen(s1) == minishell_strlen(s2))
	{
		if (!minishell_strncmp(s1, s2, minishell_strlen(s1)))
			return (true);
	}
	return (false);
}
char		*minishell_strjoin(const char *s1, const char *s2)
{
	size_t	total_len;
	char	*res;

	if (!s1)
		return ((char *)s2);
	if (!s2)
		return ((char *)s1);
	total_len = minishell_strlen(s1) + minishell_strlen(s2);
	res = (char *)malloc(sizeof(char) * (total_len + 1));
	if (res == NULL)
		return (NULL);
	minishell_strlcpy(res, s1, minishell_strlen(s1) + 1);
	minishell_strlcat(res, s2, total_len + 1);
	return (res);
}

int32_t	minishell_strcmp(const char *s1, const char *s2)
{
	uint32_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i += 1;
	return (s1[i] - s2[i]);
}
