/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i32tostr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:42 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:33:42 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

static int32_t	itoa_count_digits(int32_t nbr)
{
	int32_t	num_digits;

	num_digits = 1;
	while (nbr >= 10)
	{
		nbr /= 10;
		num_digits++;
	}
	return (num_digits);
}

static void	i32tostr_init(int32_t *nbr, char **str, int32_t *d, bool *is_neg)
{
	*is_neg = false;
	*d = 0;
	if (*nbr < 0)
	{
		*is_neg = true;
		*nbr *= -1;
		(*d)++;
	}
	*d += itoa_count_digits(*nbr);
	*str = (char *)minishell_calloc(1, (*d + 1) * sizeof(char));
}

char	*minishell_i32tostr(int32_t nbr)
{
	bool	is_negative;
	int32_t	num_digits;
	int32_t	i;
	char	*str;

	i32tostr_init(&nbr, &str, &num_digits, &is_negative);
	if (!str)
		return (NULL);
	str[num_digits] = 0;
	i = num_digits - 1;
	while (i >= (int32_t)is_negative)
	{
		str[i] = (nbr % 10) + '0';
		nbr /= 10;
		i--;
	}
	if (is_negative)
		str[0] = '-';
	return (str);
}

uint8_t	minishell_strtoui8(char *str)
{
	uint8_t		res;
	int			sign;

	res = 0;
	sign = 1;
	while (minishell_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while ('0' <= *str && *str <= '9')
	{
		res = res * 10 + (*str - 48);
		str++;
	}
	return (res * sign);
}
