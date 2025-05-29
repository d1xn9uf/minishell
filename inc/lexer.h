/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:35:02 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:35:03 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

// CMD AND WORD SEPARATORS
# define METACHARACTER "|&()<> "
# define CMDSEP "|&()><"

t_status		minishell_lexer(t_minishell *minishell);
t_status		lexer_cmd_spaced(t_lexer *lexer);
t_status		lexer_cmd_split(t_lexer *lexer);
t_status		lexer_lex(t_lexer *lexer);
t_status		lex_add_token(t_lexer *lexer, char *tvalue, t_token_type ttype);
t_token			*lex_last_token(t_token *token);
t_status		lexer_validate(t_token *token, t_status *status, int32_t *hdoc_count);
t_token_type	lex_get_token_type(char *token_vlaue);
uint32_t		minishell_getokensize(t_token *token);
void			move_token(t_token **head, uint32_t stid, uint32_t dtid);

#endif