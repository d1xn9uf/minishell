#include "../../inc/parser.h"

static void		get_flags(bool *flags, char *s);
static t_status	separate(t_token *token, char **splits);

t_status	minishell_separate(t_token *token)
{
	bool		*flags;
	char		**splits;
	t_status	status;

	if (*token->tvalue)
	{
		flags = (bool *)malloc(sizeof(bool) * minishell_strlen(token->tvalue));
		if (!flags)
			return (STATUS_MALLOCERR);
		get_flags(flags, token->tvalue);
		splits = minishell_split(token->tvalue, SPACE, flags);
		free(flags);
		if (!splits)
			return (STATUS_MALLOCERR);
		if ((status = separate(token, splits)))
			return (status);
	}
	return (STATUS_SUCCESS);
}

static void	get_flags(bool *flags, char *s)
{
	uint32_t	i;
	bool		flag[2];

	i = 0;
	flag[0] = false;
	flag[1] = false;
	while (s[i])
	{
		if (s[i] == CHAR_DOUBLE_QUOTE && !flag[0])
			flag[1] = !flag[1];
		else if (s[i] == CHAR_SINGLE_QUOTE && !flag[1])
			flag[0] = !flag[0];
		flags[i] = false;
		if (s[i] == SPACE)
			flags[i] = !flag[0] && !flag[1];
		i += 1;
	}
}

static t_status	separate(t_token *token, char **splits) // need to protect all funcs ??
{
	t_token		*last_right;
	char		*value;
	uint32_t	i;

	last_right = token->right;
	value = minishell_strdup(splits[0]);
	if (!value)
		return (minishell_free_arr(splits), STATUS_MALLOCERR);
	free(token->tvalue);
	token->tvalue = value;
	i = 1;
	while (splits[i])
	{
		token->right = (t_token *)malloc(sizeof(t_token));
		if (!token->right)
			return (token->right = last_right, minishell_free_arr(splits), STATUS_MALLOCERR);
		token->right->tvalue = minishell_strdup(splits[i]);
		if (!token->right->tvalue)
		{
			free(token->right);
			return (token->right = last_right, minishell_free_arr(splits), STATUS_MALLOCERR);
		}
		token->right->ttype = TTOKEN_ARGUMENT;
		i += 1;
		token = token->right;
	}
	return (token->right = last_right, minishell_free_arr(splits), STATUS_SUCCESS);
}
