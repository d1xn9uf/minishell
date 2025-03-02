#include "../../inc/lexer.h"

static void	cmd_split_size(const char *s, uint64_t *count)
{
	bool	flag;
	char	quote;

	flag = false;
	*count = 1;
	while (*s)
	{
		if (*s == CHAR_SINGLE_QUOTE || *s == CHAR_DOUBLE_QUOTE)
		{
			quote = *s++;
			while ((*s && *s != quote) || (*s && *s != SPACE))
				s++;
			s++;
			(*count)++;
			continue ;
		}
		if (*s != SPACE && !flag)
		{
			(*count)++;
			flag = true;
		}
		else if (*s == SPACE)
			flag = false;
		s++;
	}
}

void	cmd_split_quoted(char **spaced, t_lexer *lexer,
			uint32_t element)
{
	char		quote;
	uint64_t	len;

	len = 1;
	quote = *(*spaced);
	while (*((*spaced) + len) != quote)
		len++;
	len += 1;
	if (*((*spaced) + len) && *((*spaced) + len) != SPACE)
	{
		while (*((*spaced) + len) && *((*spaced) + len) != SPACE)
			len++;
	}
	lexer->spaced_arr.spaced_cmdline_arr[element]
		= (char *)minishell_calloc(len + 1, 1);
	minishell_strlcpy(lexer->spaced_arr.spaced_cmdline_arr[element],
		*(spaced), len + 1);
	*spaced += len;	
}

static t_status	cmd_split_word(char **spaced, t_lexer *lexer,
	uint32_t element)
{
	uint64_t	len;

	len = 0;
	if (**spaced == CHAR_SINGLE_QUOTE || **spaced == CHAR_DOUBLE_QUOTE)
	{
		cmd_split_quoted(spaced, lexer, element);
		return (STATUS_SUCCESS);
	}
	while (*((*spaced) + len) && *((*spaced) + len) != SPACE)
		len++;
	lexer->spaced_arr.spaced_cmdline_arr[element]
		= (char *)minishell_calloc(len + 1, 1);
	if (!lexer->spaced_arr.spaced_cmdline_arr[element])
		return (STATUS_MALLOCERR);
	minishell_strlcpy(lexer->spaced_arr.spaced_cmdline_arr[element],
		*spaced, len + 1);
	*spaced += len;
	return (STATUS_SUCCESS);
}

static t_status	init_split(t_lexer *lexer)
{
	cmd_split_size(lexer->spaced.spaced_cmdline, &lexer->spaced_arr.sz);
	lexer->spaced_arr.spaced_cmdline_arr = (char **)minishell_calloc(
			lexer->spaced_arr.sz, sizeof(char *));
	if (!lexer->spaced_arr.spaced_cmdline_arr)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

t_status	lexer_cmd_split(t_lexer *lexer)
{
	t_status	status;
	uint32_t	count;
	char		*spaced;

	status = init_split(lexer);
	if (status)
		return (status);
	count = 0;
	spaced = lexer->spaced.spaced_cmdline;
	while (*spaced)
	{
		while (minishell_isspace(*spaced))
			spaced++;
		if (!*spaced)
			break ;
		status = cmd_split_word(&spaced, lexer, count);
		if (status)
			return (status);
		count++;
	}
	lexer->spaced_arr.spaced_cmdline_arr[count] = 0;
	lexer->sztoken = count;
	return (STATUS_SUCCESS);
}
