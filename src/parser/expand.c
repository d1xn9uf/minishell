#include "../../inc/parser.h"

static char	*extract_key(char *content, uint32_t **index);

char	*minishell_expand(char *content, t_env *env) //9ssem a jemmi hhh
{
	static char		*result;
	static char		*save;
	static char		*temp;
	static char		*key;
	static char		*value;
	static char		*group;
	static uint32_t	s;
	static uint32_t	e;
	
	while (content[s])
	{
		while (content[e] && content[e] != CHAR_DOLLAR_SIGN)
			e += 1;
		content[e] = 0;
		temp = minishell_strdup(content + s);
		content[e] = CHAR_DOLLAR_SIGN;
		if (!temp)
			return (NULL);
		key = extract_key(content, &e);
		if (!key)
			return (free(temp), NULL);
		value = minishell_getvalue(env, key);
		free(key);
		if (!value)
			return (free(temp), NULL);
		if (!result)
			result = minishell_strjoin(temp, value); // check return
		else
		{
			save = result;
			group = minishell_strjoin(temp, value);
			result = minishell_strjoin(result, group);
			free(save);
			free(group);
		}
		free(temp);
		free(value);
		s = e;
	}
}

static char	*extract_key(char *content, uint32_t **index)
{
	uint32_t	start;

	start = *index;
	
}
