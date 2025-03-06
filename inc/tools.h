#ifndef TOOLS_H
# define TOOLS_H

# include "minishell.h"

// STRING FUNCTIONS
bool		minishell_strchr(const char *s, int c);
uint32_t	minishell_strlen(const char *s);
uint64_t	minishell_strlcpy(char *dst, const char *src,
				uint64_t dstsize);

int32_t		minishell_strncmp(const char *s1,
				 const char *s2, size_t n);
char		*minishell_strdup(const char *s);
bool		minishell_strequal(const char *s1, const char *s2);
char		*minishell_strjoin(const char *s1, const char *s2);

char		**minishell_split(char *s, char c);
size_t		minishell_strlcat(char *dst, const char *src, size_t size);

// MEMORY UTILS
void		*minishell_memset(void *b, int c, size_t len);
void		*minishell_calloc(size_t count, size_t size);

// IS FUNCTIONS
bool		minishell_isspace(int c);
bool		minishell_iscmdsep(int c);

// ERROR HANDLING
void		minishell_error(t_status status);

// CLEAN
void   		minishell_reset(t_minishell **minishell);
void		minishell_free_arr(char **arr);

// ENVIRONMENT UTILS
t_env		*minishell_envlist(int32_t count);
int32_t		minishell_envsize(char **env);
void		minishell_freekv(t_env *l_env, int32_t index);
void		minishell_freelst(t_env *l_env);

t_env		*minishell_getenv(char **env);
char		*minishell_getvalue(t_env *env, char *key);

char		*minishell_getpath(t_env *env, char *cmd);

// REDIRECTION UTILS 
bool		minishell_isred(t_root *node);
char		*minishell_generate_filename();

#endif