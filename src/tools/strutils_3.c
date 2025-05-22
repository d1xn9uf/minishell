/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 20:51:41 by mzary             #+#    #+#             */
/*   Updated: 2025/05/13 20:53:49 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

char	*minishell_strjoin3(char *s1, char *s2, char *s3)
{
	char	*res1;
	char	*res2;

	res1 = minishell_strjoin(s1, s2);
	if (!res1)
		return (NULL);
	res2 = minishell_strjoin(res1, s3);
	minishell_free((void **)&res1);
	return (res2);
}
