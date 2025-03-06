# include "../../inc/minishell.h"

void    reset_token(t_root *root)
{
    if (root)
    {
        reset_token(root->left);
        reset_token(root->right);
        //if (root->tvalue)
         //   free(root->tvalue);
        free(root);
    }
}

void    reset_lexer(t_minishell *minishell)
{
    if (minishell)
    {
        if (minishell->lexer->spaced.spaced_cmdline)
            free(minishell->lexer->spaced.spaced_cmdline);
        if (minishell->root)
            reset_token(minishell->root);
        free(minishell->lexer);
    }
}

void    minishell_reset(t_minishell **minishell)
{
    if (minishell && *minishell)
    {
        if ((*minishell)->lexer)
            reset_lexer(*minishell);
        if ((*minishell)->cmdline)
            free((*minishell)->cmdline);
        (*minishell)->exit_code = 0;
    }
}
