/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterisk.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:34:08 by mzary             #+#    #+#             */
/*   Updated: 2025/05/10 17:38:50 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"

static t_status	add_name(t_match **ns, char *s);
static void		free_mem(t_match *ns, t_fixe *fixe);
static t_status	add_to_tree(t_token *token, t_match *ns);
static t_status	add_name_to_tree(t_norm_ast *local);

t_status	minishell_asterisk(t_token *token, bool *asterisk)
{
	t_match			*ns;
	t_fixe			*fixe;
	DIR				*dirp;
	struct dirent	*entry;
	char			*n;

	ns = NULL;
	fixe = minishell_analyse(token->tvalue, asterisk);
	if (!fixe)
		return (STATUS_MALLOCERR);
	/* debug */
	int i = 0;
	printf("count of fixes is %u\n", fixe->count);
	while (fixe->fixes[i])
	{
		printf("[%s]\n", fixe->fixes[i]);
		i += 1;
	}
	/* debug */
	dirp = opendir(".");
	if (!dirp)
		return (free_mem(NULL, fixe), STATUS_FAILURE);
	entry = readdir(dirp);
	while (entry)
	{
		n = entry->d_name;
		if (!minishell_strequal(n, ".") && !minishell_strequal(n, ".."))
		{
			if (minishell_matcher(fixe, n) && add_name(&ns, n))
				return (free_mem(ns, fixe), closedir(dirp), STATUS_MALLOCERR);
		}
		entry = readdir(dirp);
	}
	return (free_mem(NULL, fixe), closedir(dirp), add_to_tree(token, ns));
}

static t_status	add_name(t_match **ns, char *s)
{
	t_match	*match;
	t_match	*last;

	match = (t_match *)malloc(sizeof(t_match));
	if (!match)
		return (STATUS_MALLOCERR);
	match->name = minishell_strdup(s);
	if (!match->name)
		return (minishell_free((void **)&match), STATUS_MALLOCERR);
	match->next = NULL;
	if (!*ns)
		return (*ns = match, STATUS_SUCCESS);
	last = *ns;
	while (last->next)
		last = last->next;
	last->next = match;
	return (STATUS_SUCCESS);
}

static t_status	add_to_tree(t_token *token, t_match *ns)
{
	t_norm_ast	local;
	t_status	status;

	local.ns = ns;
	local.rright = token->right;
	local.first = true;
	local.saver = ns;
	local.cur = token;
	while (local.ns)
	{
		status = add_name_to_tree(&local);
		if (status)
			return (status);
	}
	return (free_mem(local.saver, NULL), STATUS_SUCCESS);
}

static t_status	add_name_to_tree(t_norm_ast *local)
{
	local->rep = minishell_strdup(local->ns->name);
	if (!local->rep)
		return (free_mem(local->ns, NULL), STATUS_MALLOCERR);
	if (!local->first)
		local->cur->ttype = TTOKEN_ARGUMENT;
	if (local->first && minishell_free((void **)&local->cur->tvalue))
		local->first = false;
	local->cur->tvalue = local->rep;
	local->ns = local->ns->next;
	if (local->ns)
	{
		local->cur->right = (t_token *)minishell_calloc(1, sizeof(t_token));
		if (!local->cur->right)
		{
			local->cur->right = local->rright;
			return (free_mem(local->ns, NULL), STATUS_MALLOCERR);
		}
		local->cur = local->cur->right;
		local->cur->left = NULL;
		local->cur->is_filename = true;
	}
	else
		local->cur->right = local->rright;
	return (STATUS_SUCCESS);
}

static void	free_mem(t_match *ns, t_fixe *fixe)
{
	t_match	*next;

	while (ns)
	{
		next = ns->next;
		minishell_free((void **)&ns->name);
		minishell_free((void **)&ns);
		ns = next;
	}
	if (fixe)
	{
		minishell_free_arr(fixe->fixes);
		minishell_free((void **)&fixe->flags);
		minishell_free((void **)&fixe);
	}
}
