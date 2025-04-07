#include "../../inc/builtins.h"

t_status	minishell_exit(t_minishell *minishell)
{
	minishell_cleanup(minishell, STATUS_SUCCESS);
	return (STATUS_SUCCESS);
}
