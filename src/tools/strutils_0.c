#include "../../inc/tools.h"

bool	minishell_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return (true);
		s++;
	}
	return (false);
}

bool	minishell_isspace(int c)
{
	return (c == 32);
}

uint32_t	minishell_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*(s + len))
		len++;
	return (len);
}

bool	minishell_iscmdsep(int c)
{
	if (minishell_strchr(CMDSEP, c))
		return (true);
	return (false);
}


uint64_t	minishell_strlcpy(char *dst, const char *src, uint64_t dstsize)
{
	uint64_t	i;
	uint64_t	src_len;
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
