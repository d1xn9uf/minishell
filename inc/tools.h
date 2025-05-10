/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:57 by mzary             #+#    #+#             */
/*   Updated: 2025/05/10 17:51:24 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_H
# define TOOLS_H

# include "minishell.h"

# define FREAD_SIZE 1024
# define FNAME_SIZE 20

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

// MEMORY UTILS
void		*minishell_memset(void *b, int c, size_t len);
void		*minishell_calloc(size_t count, size_t size);
bool		minishell_free(void **p);

// IS FUNCTIONS
bool		minishell_isspace(int c);
bool		minishell_iscmdsep(int c);

// ERROR HANDLING
void		minishell_error(t_status status);

// CLEAN
void		minishell_reset(t_minishell *minishell);
void		minishell_cleanup(t_minishell *minishell, int32_t exit_status);
void		minishell_free_arr(char **arr);
void		minishell_free_token(t_token *token);

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

// REDIRECTION UTILS 
bool		minishell_isred(t_root *node);
t_status	hdoc_keyword_noquotes(char **keyword);
char		*minishell_generate_filename(void);
char		*minishell_readfile(char *filename);

// SIGNALS
t_status	minishell_siginit(void);

// EXIT STATUS
void		minishell_setstatus(t_minishell *minishell, int status);

// TREE UTILS
t_status	minishell_protect(t_token *root);

// NORM UTILS
bool		setuint32(uint32_t *i, uint32_t v);
bool		setchar(char *c, char v);
bool		setbool(bool *flag, bool value);

#endif