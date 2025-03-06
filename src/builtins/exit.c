#include "../../inc/builtins.h"

t_status	minishell_exit(t_minishell *minishell)
{
	minishell_reset(&minishell);
	exit(STATUS_SUCCESS);
}
