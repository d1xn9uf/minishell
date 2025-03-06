#include "../../inc/parser.h"

static uint32_t	count_asterisks(char *value);
static void		decide_asterisk(char *value, bool *asterisk);
static char		*assemble_chunks(t_chunk *chunks);
static uint32_t	count_size(t_chunk *chunks);

t_status	minishell_interpret(t_token *token, t_env *env)
{
	uint32_t	a_count; //remove if norminette
	bool		*asterisk;
	t_chunk		*chunks;
	char		*pf_value;

	a_count = count_asterisks(token->tvalue);
	asterisk = (bool *)malloc(sizeof(bool) * a_count); // if a_count = 0 it is okay
	if (!asterisk)
		return (STATUS_MALLOCERR);
	decide_asterisk(token->tvalue, asterisk);
	chunks = minishell_chunker(token->tvalue, env);
	if (!chunks)
		return (free(asterisk), STATUS_MALLOCERR);
	pf_value = assemble_chunks(chunks);
	free_chunks(chunks);
	if (!pf_value)
		return (free(asterisk), STATUS_MALLOCERR);
	free(token->tvalue);
	token->tvalue = pf_value;	
	if (minishell_asterisk(token, asterisk)) // fix condition later and function
		return (free(asterisk), STATUS_FAILURE);
	return (free(asterisk), STATUS_SUCCESS);
}

static uint32_t	count_asterisks(char *value)
{
	uint32_t	count;

	count = 0;
	while (*value)
	{
		if (*value == '*')
			count += 1;
		value += 1;
	}
	return (count);
}

static void		decide_asterisk(char *value, bool *asterisk)
{
	bool		flag[2];
	uint32_t	count;

	flag[0] = false;
	flag[1] = false;
	count = 0;
	while (*value)
	{
		if (*value == CHAR_DOUBLE_QUOTE && !flag[0])
			flag[1] = !flag[1];
		else if (*value == CHAR_SINGLE_QUOTE && !flag[1])
			flag[0] = !flag[0];
		else if (*value == '*')
		{
			asterisk[count] = (!flag[0] && !flag[1]);
			count += 1;
		}
		value += 1;
	}
}

static char		*assemble_chunks(t_chunk *chunks)
{
	uint32_t	size;
	char		*pf_value;
	uint32_t	i;
	uint32_t	j;

	size = count_size(chunks);
	pf_value = (char *)malloc(sizeof(char) * (size + 1));
	if (!pf_value)
		return (NULL);
	i = 0;
	while (i < size)
	{
		j = 0;
		while (chunks->content[j])
		{
			pf_value[i] = chunks->content[j];
			i += 1;
			j += 1;
		}
		chunks = chunks->next;
	}
	pf_value[i] = 0;
	return (pf_value);
}

static uint32_t	count_size(t_chunk *chunks)
{
	uint32_t	count;
	uint32_t	i;

	count = 0;
	while (chunks)
	{
		i = 0;
		while (chunks->content[i])
		{
			count += 1;
			i += 1;
		}
		chunks = chunks->next;
	}
	return (count);
}
