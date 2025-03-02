#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

// CMD AND WORD SEPARATORS
# define METACHARACTER "|&()<> "
# define CMDSEP "|&()><"

t_status		minishell_lexer(t_minishell *minishell);
t_status		lexer_cmd_spaced(t_lexer *lexer);
t_status    	lexer_cmd_split(t_lexer *lexer);
t_status		lexer_lex(t_lexer *lexer);
t_status		lex_add_token(t_lexer *lexer, char *tvalue, t_token_type ttype);
t_token			*lex_last_token(t_token *token);
t_status		lexer_validate(t_token *token);
t_token_type	lex_get_token_type(char *token_vlaue);

#endif