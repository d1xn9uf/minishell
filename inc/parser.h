/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:35:00 by mzary             #+#    #+#             */
/*   Updated: 2025/05/23 19:52:31 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

t_status	minishell_parser(t_minishell *minishell);

t_status	minishell_translate(t_token *root, t_env *env, char *str_exitcode);

t_status	update_command(t_token *token, t_env *env);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args);

t_status	interpret_dollar(t_token *token, t_env *env, t_args *args);
char		*minishell_concatenate(t_substr *head);

char		*minishell_expand(char *content, t_env *env, t_args args);
bool		ignore(t_result *buff, uint32_t i);

t_status	minishell_separate(t_token *token);

t_status	minishell_asterisk(t_token *token);
void		minishell_order(t_match **names);

t_fixe		*minishell_analyse(t_token *token);

bool		minishell_matcher(t_fixe *fixe, char *s);

t_status	minishell_remove(t_token *token);

#endif