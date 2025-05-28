/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:57 by mzary             #+#    #+#             */
/*   Updated: 2025/05/28 09:48:23 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_H
# define TOOLS_H

# include "minishell.h"

# define MAX_FD 1024
# define FREAD_SIZE 1024
# define FNAME_SIZE 20
# define MAX_SINT64	"9223372036854775807"
# define MIN_SINT64	"9223372036854775808"

// STRING FUNCTIONS
bool		minishell_strchr(const char *s, int c);
uint32_t	minishell_strlen(const char *s);
uint64_t	minishell_strlcpy(char *dst, const char *src,
				uint64_t dstsize);

int32_t		minishell_strncmp(const char *s1, const char *s2, size_t n);
char		*minishell_strdup(const char *s);
bool		minishell_strequal(const char *s1, const char *s2);
char		*minishell_strjoin(const char *s1, const char *s2);
int32_t		minishell_strcmp(const char *s1, const char *s2);

char		**minishell_split(char *s, char c, bool *flags);
size_t		minishell_strlcat(char *dst, const char *src, size_t size);
bool		is_separator(char c);

char		*minishell_i32tostr(int32_t nbr);
uint8_t		minishell_strtoui8(char *str);

char		*minishell_strjoin3(char *s1, char *s2, char *s3);
char		*minishell_substr(char *str, uint32_t s, uint32_t e);
uint32_t	minishell_count(char *s, char c);

// MEMORY UTILS
void		*minishell_memset(void *b, int c, size_t len);
void		*minishell_calloc(size_t count, size_t size);
bool		minishell_free(void **p);

// IS FUNCTIONS
bool		minishell_isspace(int c);
bool		minishell_iscmdsep(int c);
bool		minishell_iscmdop(t_token *token);

// ERROR HANDLING
void		minishell_error(t_status status);
bool		minishell_stderr(char *head, char *middle, char *till);
bool		minishell_stderr2(char *head, char *middle, char *middle2,
				char *middle3, char *till);

// CLEAN
void		minishell_reset(t_minishell *minishell);
void		minishell_cleanup(t_minishell *minishell, int32_t exit_status);
void		cleanup_fds(t_minishell *minishell, bool closestd);
void		minishell_free_arr(char **arr);
void		minishell_free_token(t_token *token);
void		reset_token(t_root *root);

// ENVIRONMENT UTILS
t_env		*minishell_envlist(int32_t count);
int32_t		minishell_envsize(char **env);
void		minishell_freekv(t_env *l_env, int32_t index);
void		minishell_freelst(t_env *l_env);
char		*minishell_unquoted(t_env *env, char *key);

t_env		*minishell_getenv(char **env);
char		*minishell_getvalue(t_env *env, char *key);
void		minishell_envfree(t_env *env);

char		*minishell_getpath(t_env *env, char *cmd, t_status *status);

char		**minishell_getenvp(t_env *env);
bool		minishell_validkey(char *key);

// CHAR UTILS
bool		is_num(char c);
bool		is_up(char c);
bool		is_low(char c);
bool		is_anum(char c);

// REDIRECTION UTILS 
void		lex_reposition_red(t_lexer *lexer);
bool		minishell_isred(t_root *node);
t_status	hdoc_keyword_noquotes(char **keyword);
char		*minishell_generate_filename(void);
char		*minishell_readfile(char *filename);

// SIGNALS
t_status	minishell_siginit(void);
void		sigint_hdoc(int32_t signum);

// EXIT STATUS
void		minishell_setstatus(t_minishell *minishell, int status);

// TREE UTILS
t_status	minishell_protect(t_token *root);
void		fix_tree(t_token *token);
void		clean_tree(t_token *token);
bool		minishell_freesubs(t_substr **head);

// NORM UTILS
bool		setuint32(uint32_t *i, uint32_t v);
bool		setchar(char *c, char v);
bool		setbool(bool *flag, bool value);

#endif