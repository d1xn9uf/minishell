#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef enum e_default_priority
{
	PRIORITY_CRITICAL = 1,
	PRIORITY_HIGH,
	PRIORITY_MEDIUM,
	PRIORITY_IDLE,
	PRIORITY_REMOVE
}	t_default_priority;

typedef struct s_args
{
	char	*exit;
	bool	flag;
	uint8_t	step;
	bool*	ec_usedp;
}	t_args;

typedef struct s_chunk
{
	char			*content;
	struct s_chunk	*next;
}	t_chunk;

typedef struct s_result
{
	char	*key;
	char	*value;
	char	*prefix;
	char	*suffix;
	char	*saver;
	char	*result;
	bool	flag[2];
}	t_result;

typedef struct s_ast
{
	bool	before;
	bool	after;
}	t_ast;

typedef struct s_fixe
{
	char		**fixes;
	t_ast		*flags;
	uint32_t	count;
}	t_fixe;

typedef struct s_quotes
{
	bool		*flags;
	uint32_t	fsize;
	uint32_t	i[3];
	char		*fvalue;
}				t_quotes;

t_status	minishell_parser(t_minishell *minishell);

t_status	minishell_translate(t_token *root, t_env *env, char *str_exitcode);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args);
bool		setbool(bool *flag, bool value);

char		*minishell_expand(char *content, t_env *env, t_args args);

t_status	minishell_separate(t_token *token);

t_status	minishell_asterisk(t_token *token, bool *asterisk);

t_fixe		*minishell_analyse(char *pattern, bool *asterisk);

bool		minishell_matcher(t_fixe *fixe, char *s);

t_status	minishell_remove(t_token *token);

#endif