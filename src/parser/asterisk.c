#include "../../inc/parser.h"

typedef struct s_match
{
	char			*name;
	struct s_match	*next;
}	t_match;

static t_status	add_name(t_match **names, char *s);
static void		free_mem(t_match *names, t_fixe *fixe);
static t_status	add_to_tree(t_token *token, t_match *names);

t_status	minishell_asterisk(t_token *token, bool *asterisk)
{
	t_match			*names;
	t_fixe			*fixe;
	DIR				*dirp;
	struct dirent	*entry;

	names = NULL;
	fixe = minishell_analyse(token->tvalue, asterisk);
	if (!fixe)
		return (STATUS_MALLOCERR);
	dirp = opendir(".");
	if (!dirp)
		return (free_mem(NULL, fixe), STATUS_FAILURE); // custom status pls? :<)
	entry = readdir(dirp);
	while (entry)
	{
		if (entry->d_name[0] != '.' && minishell_matcher(fixe, entry->d_name))
		{
			if (add_name(&names, entry->d_name))
				return (free_mem(names, fixe) , closedir(dirp), STATUS_MALLOCERR);
		}
		entry = readdir(dirp);
	}
	return (free_mem(NULL, fixe), closedir(dirp), add_to_tree(token, names));
}

static t_status	add_name(t_match **names, char *s)
{
	t_match	*match;
	t_match	*last;

	match = (t_match *)malloc(sizeof(t_match));
	if (!match)
		return (STATUS_MALLOCERR);
	match->name = minishell_strdup(s); // s is not in heap, read documentation
	if (!match->name)
		return (free(match), STATUS_MALLOCERR);
	match->next = NULL;
	if (!*names)
		return (*names = match, STATUS_SUCCESS);
	last = *names;
	while (last->next)
		last = last->next;
	last->next = match;
	return (STATUS_SUCCESS);
}

static void		free_mem(t_match *names, t_fixe *fixe)
{
	t_match	*next;

	while (names)
	{
		next = names->next;
		free(names->name);
		free(names);
		names = next;
	}
	if (fixe)
	{
		minishell_free_arr(fixe->fixes);
		free(fixe->flags);
		free(fixe);
	}
}

static t_status	add_to_tree(t_token *token, t_match *names) // gad had lkhra - m0hc33n
{
		t_token *rright;
		bool first;
		char *rep;
		t_match	*saver;
		t_token *cur;
		
		rright = token->right;
		first = true;
		cur = token;
		saver = names;
		while (names)
		{
			//fprintf(stderr, "[%p]\n", names->name);
			rep = minishell_strdup(names->name);
			if (!rep)
				return (free_mem(names, NULL), STATUS_MALLOCERR);
			if (first)
			{
				free(cur->tvalue);
				first = false;
			}
			cur->tvalue = rep;
			cur->ttype = TTOKEN_ARGUMENT;
			names = names->next;
			if (names)
			{
				cur->right = (t_token *)malloc(sizeof(t_token));
				if (!cur->right)
				{
					cur->right = rright;
					return (free_mem(names, NULL), STATUS_MALLOCERR);
				}
				cur = cur->right;
				cur->left = NULL;
			}
			else
				cur->right = rright;
		}
		return (free_mem(saver, NULL), STATUS_SUCCESS);
}
