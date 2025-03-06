#include "../../inc/parser.h"

static t_status update(t_token *token, t_env *env);
static void		fix_tree(t_token *root);
static t_status clean_tree(t_token *token);

t_status	minishell_translate(t_token *root, t_env *env)
{
    t_status    status;

    if ((status = update(root, env)))
    	return (status);
	fix_tree(root);
    if ((status = clean_tree(root)))
    	return (status);
    return (STATUS_SUCCESS);
}

static t_status    update(t_token *token, t_env *env)
{
    t_status    status;
	char		*temp;

    if (token)
    {
		if ((status = minishell_interpret(token, env)))
			return (status);
		if (token->ttype == TTOKEN_COMMAND)
		{
			temp = minishell_getpath(env, token->tvalue);
			if (!temp)
				return (STATUS_CMDFAILED);
			free(token->tvalue);
			token->tvalue = temp;
		}
        if ((status = update(token->right, env)))
            return (status);
        return (STATUS_SUCCESS);
    }
    return (STATUS_SUCCESS);
}

static void	fix_tree(t_token *root)
{
	t_token		*next;

	if (root->right && !root->right->tvalue)
	{
		next = root->right->right;
		free(root->right);
		root->right = next;
	}
	if (root->right)
		fix_tree(root->right);
}

static t_status    clean_tree(t_token *token)
{
    t_status    status;

    if (token)
    {
        if (token->left && (token->left->ttype == TTOKEN_PARENTHESE_CLOSE 
		|| token->left->ttype == TTOKEN_PARENTHESE_OPEN))
        {
            free(token->left);
            token->left = NULL;
        }
        if (token->right && (token->right->ttype == TTOKEN_PARENTHESE_CLOSE 
        || token->right->ttype == TTOKEN_PARENTHESE_OPEN))
        {
            free(token->right);
            token->right = NULL;
        }
        if ((status = clean_tree(token->left)))
            return (status);
        if ((status = clean_tree(token->right)))
            return (status);
        return (STATUS_SUCCESS);
    }
    return (STATUS_SUCCESS);
}
