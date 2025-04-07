#include "../../inc/tools.h"

void	*minishell_memset(void *b, int c, size_t len)
{
	while (len--)
	{
		*(unsigned char *)(b + len) = (unsigned char)c;
	}
	return (b);
}

void	*minishell_calloc(size_t count, size_t size)
{
	void	*ptr;

	if ((int)count < 0 && (int)size < 0)
		return (0);
	ptr = malloc(count * size);
	if (!ptr)
		return (0);
	minishell_memset(ptr, 0, count * size);
	return (ptr);
}

bool	minishell_free(void **p)
{
	if (*p)
	{
		free(*p);
		*p = NULL;
	}
	return (true);
}
