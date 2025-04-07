#include "../../inc/tools.h"

void    minishell_free_arr(char **arr)
{
    uint64_t    i;

    i = 0;
    if (arr)
    {
        while (arr[i])
        {
            free(arr[i]);
			arr[i] = NULL;
            i++;
        }
        free(arr);
		arr = NULL;
    }
}

void	minishell_cleanup(t_minishell *minishell, int32_t exit_status)
{
	minishell_reset(minishell);
	close(minishell->stdfd[0]);
	close(minishell->stdfd[1]);
	minishell_envfree(minishell->env);
	free(minishell->cwd);
	free(minishell);
	rl_clear_history();
	exit(exit_status);
}
