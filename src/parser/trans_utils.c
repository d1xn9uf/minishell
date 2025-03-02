#include "../../inc/parser.h"

static uint32_t count_removed(char *s);
static uint32_t decide(char c, uint8_t *flag);

char    *remove_quotes(char *s)
{
    uint32_t    final_size;
    char        *final_value;
    uint32_t     i;
    uint8_t      flag[2];

    final_size = count_removed(s);
    final_value = (char *)malloc(sizeof(char) * (final_size + 1));
    if (!final_size)
        return (NULL);
    i = 0;
    flag[0] = 0;
    flag[1] = 0;
    while (i < final_size)
    {
        if (decide(*s, flag))
        {
            final_value[i] = *s;
            i += 1;
        }
        s += 1;
    }
    final_value[i] = 0;
    return (final_value);
}

static uint32_t count_removed(char *s)
{
    uint32_t    count;
    uint32_t    i;
    uint8_t     flag[2];

    count = 0;
    i = 0;
	flag[0] = 0;
	flag[1] = 0;
    while (s[i])
	{
		count += decide(s[i], flag);
		i += 1;
	}
    return (count);
}

static uint32_t decide(char c, uint8_t *flag)
{
	if (c == CHAR_SINGLE_QUOTE)
	{
		if (flag[1])
			return (1);
		else
			return (flag[0]= 1 - flag[0], 0);
	}
	else if (c == CHAR_DOUBLE_QUOTE)
	{
		if (flag[0])
			return (1);
		else
			return (flag[1] = 1 - flag[1], 0);
	}
    else
        return (1);
}
