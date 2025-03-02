#include "../../inc/tools.h"

static bool		matches_pattern(char *pattern, char *s);
static t_status	add_token(t_token **list, char *s);
static void		link_to_token(t_token *token, t_token *list);

t_status	minishell_asterisk(t_token *token) // check if u need to interpret * inside quotes eagle eye baby
{
	DIR				*dirp;
	t_token			*tokens_add;
	struct dirent	*entry;

	dirp = opendir(".");
	if (!dirp)
		return (STATUS_FAILURE); // custom status pls? :<)
	tokens_add = NULL;
	entry = readdir(dirp);
	while (entry)
	{
		if (matches_pattern(token->tvalue, entry->d_name))
		{
			if (add_token(&tokens_add, entry->d_name))
				return (STATUS_MALLOCER);
		}
		entry = readdir(dirp);
	}
	link_to_token(token, tokens_add);
	closedir(dirp);
}

static bool		matches_pattern(char *pattern, char *s)
{
	
}

static t_status	add_token(t_token **list, char *s)
{

}

static void		link_to_token(t_token *token, t_token *list)
{

}
