#include "../../inc/tools.h"

void	minishell_error(t_status status)
{
    if (status != STATUS_EMPTYCMD)
        printf("MINISHELL ERROR : %#06x\n", status);
}
