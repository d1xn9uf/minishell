/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:05 by mzary             #+#    #+#             */
/*   Updated: 2025/05/10 15:42:42 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static char		*expand(t_result *buff, t_env *env, t_args args);
static t_status	modify(t_result *buff, t_env *env, t_args args, uint32_t *i);
static void		flagger(t_result *buff, uint32_t *i);
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
			i[1] = i[0];
			while (setuint32(&i[1], i[1] + 1) && buff->result[i[1]])
			{
				if (!minishell_strncmp(buff->result + i[1] -1, "$?", 2)
					&& setuint32(&i[1], i[1] + 1))
					break ;
				if (is_separator(buff->result[i[1]]))
					break ;
			}
			buff->key = (char *)malloc(sizeof(char) * (i[1] - i[0] + 1));
			if (!buff->key || modify(buff, env, args, i))
				return (minishell_free((void **)&buff->result), NULL);
		}
		else
			flagger(buff, i);
	}
	return (buff->result);
}

static t_status	modify(t_result *buff, t_env *env, t_args args, uint32_t *i)
{
	minishell_strlcpy(buff->key, buff->result + i[0], i[1] - i[0] + 1);
	if (minishell_strequal(buff->key, "$?"))
		buff->value = minishell_strdup(args.exit);
	else if (minishell_strequal(buff->key, "$") && ignore(buff, i[0]))
		buff->value = minishell_strdup("");
	else
		buff->value = minishell_unquoted(env, buff->key);
	if (setchar(&buff->result[i[0]], 0) && !buff->value)
		return (free_buff(buff, 1, true), STATUS_MALLOCERR);
	buff->prefix = minishell_strdup(buff->result);
	if (setchar(&buff->result[i[0]], '$') && !buff->prefix)
		return (free_buff(buff, 2, true), STATUS_MALLOCERR);
	buff->suffix = minishell_strdup(buff->result + i[1]);
	if (free_buff(buff, 0, true) && !buff->suffix)
		return (free_buff(buff, 3, false), STATUS_MALLOCERR);
	buff->result = minishell_strjoin(buff->prefix, buff->value);
	i[0] += minishell_strlen(buff->value);
	if (free_buff(buff, 3, false) && !buff->result)
		return (free_buff(buff, 4, false), STATUS_MALLOCERR);
	buff->saver = buff->result;
	buff->result = minishell_strjoin(buff->saver, buff->suffix);
	if (free_buff(buff, 5, false) && !buff->result)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static void	flagger(t_result *buff, uint32_t *i)
{
	if (buff->result[i[0]] == CHAR_SINGLE_QUOTE && !buff->flag[1])
		buff->flag[0] = !buff->flag[0];
	else if (buff->result[i[0]] == CHAR_DOUBLE_QUOTE && !buff->flag[0])
		buff->flag[1] = !buff->flag[1];
	i[0] += 1;
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
