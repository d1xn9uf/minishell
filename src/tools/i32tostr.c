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

static void	i32tostr_init(int32_t *nbr,  char **str, int32_t *d, bool *is_neg)
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

char *minishell_i32tostr(int32_t nbr)
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
