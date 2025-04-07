#include "../../inc/parser.h"

static char		*expand(t_result *buff, t_env *env, t_args args);
static t_status	modify(t_result *buff, t_env *env, t_args args, uint32_t *i);
static bool		is_separator(char c);
static bool		free_buff(t_result *buff, uint32_t l, bool free_res);

char	*minishell_expand(char *content, t_env *env, t_args args)
{
	t_result	buff;
	
	minishell_memset(&buff, 0, sizeof(t_result));
	if (!minishell_strchr(content, CHAR_DOLLAR_SIGN))
		return (minishell_strdup(content));
	buff.result = minishell_strdup(content);
	if (!buff.result)
		return (NULL);
	return (expand(&buff, env, args));
}

static char	*expand(t_result *buff, t_env *env, t_args args)
{
	uint32_t	i[2];

	minishell_memset(&i, 0, 2 * sizeof(uint32_t));
	while (buff->result[i[0]])
	{
		if (buff->result[i[0]] == CHAR_DOLLAR_SIGN && !buff->flag[0])
		{
			i[1] = i[0] + 1;
			while (buff->result[i[1]] && !is_separator(buff->result[i[1]]))
				i[1] += 1;
			buff->key = (char *)malloc(sizeof(char) * (i[1] - i[0] + 1));
			if (modify(buff, env, args, i))
				return (NULL);
			i[0] = i[1];
		}
		else
		{
			if (buff->result[i[0]] == CHAR_SINGLE_QUOTE && !buff->flag[1])
				buff->flag[0] = !buff->flag[0];
			else if (buff->result[i[0]] == CHAR_DOUBLE_QUOTE && !buff->flag[0])
				buff->flag[1] = !buff->flag[1];
			i[0] += 1;
		}
	}
	return (buff->result);
}

static t_status	modify(t_result *buff, t_env *env, t_args args, uint32_t *i)
{
	if (!buff->key)
		return (free_buff(buff, 0, true), STATUS_MALLOCERR);
	minishell_strlcpy(buff->key, buff->result + i[0], i[1] - i[0] + 1);
	if (minishell_strequal(buff->key, "$?") && setbool(args.ec_usedp, true))
		buff->value = args.exit;
	else
		buff->value = minishell_getvalue(env, buff->key);
	if (!buff->value)
		return (setbool(args.ec_usedp, false), free_buff(buff, 1, true), STATUS_MALLOCERR);
	buff->result[i[0]] = 0;
	buff->prefix = minishell_strdup(buff->result);
	buff->result[i[0]] = CHAR_DOLLAR_SIGN;
	if (!buff->prefix)
		return (free_buff(buff, 2, true), STATUS_MALLOCERR);
	buff->suffix = minishell_strdup(buff->result + i[1]);
	if (free_buff(buff, 0, true) && !buff->suffix)
		return (free_buff(buff, 3, false), STATUS_MALLOCERR);
	buff->result = minishell_strjoin(buff->prefix, buff->value);
	if (free_buff(buff, 3, false) && !buff->result)
		return (free_buff(buff, 4, false), STATUS_MALLOCERR);
	buff->saver = buff->result;
	buff->result = minishell_strjoin(buff->saver, buff->suffix);
	if (free_buff(buff, 5, false) && !buff->result)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static bool	is_separator(char c) // !!!!! need all cases a jemmi
{
	if (minishell_isspace(c))
		return (true);
	if (c == CHAR_ASTERISK)
		return (true);
	if (c == CHAR_SINGLE_QUOTE)
		return (true);
	if (c == CHAR_DOUBLE_QUOTE)
		return (true);
	if (c == CHAR_DOLLAR_SIGN)
		return (true);
	return (false);
}

static bool	free_buff(t_result *buff, uint32_t l, bool free_res)
{
	if (l >= 1 && buff->key)
		minishell_free((void **)&buff->key);
	if (l >= 2 && buff->value)
		minishell_free((void **)&buff->value);
	if (l >= 3 && buff->prefix)
		minishell_free((void **)&buff->prefix);
	if (l >= 4 && buff->suffix)
		minishell_free((void **)&buff->suffix);
	if (l >= 5 && buff->saver)
		minishell_free((void **)&buff->saver);
	if (free_res && buff->result)
		minishell_free((void **)&buff->result);
	return (true);
}
