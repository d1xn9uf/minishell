#include "../../inc/parser.h"

static t_status update_value(t_token *token, t_env *env);
static t_status	interpret(t_env *env, t_token *token);
static t_status remove_quote(t_token *token);

t_status	minishell_translate(t_token *root, t_env *env)
{
    t_status    status;

    if ((status = update_value(root, env)))
        return (status);
    if ((status = remove_quote(root)))
        return (status);
    return (STATUS_SUCCESS);
}

static t_status    update_value(t_token *token, t_env *env)
{
    t_status    status;
    char		*temp;

    if (token)
    {
        if (token->ttype == TTOKEN_COMMAND || (token->ttype == TTOKEN_VARIABLE && !minishell_strequal(token->tvalue, "$?")) 
        	|| (token->ttype == TTOKEN_ARGUMENT && minishell_strchr(token->tvalue, '*')))
        {
            if (interpret(env, token))
                return (STATUS_FAILURE); // STATUS_PARSER
        }
        if ((status = update_value(token->left, env)))
            return (status);
        if ((status = update_value(token->right, env)))
            return (status);
        return (STATUS_SUCCESS);
    }
    return (STATUS_SUCCESS);
}

static t_status	interpret(t_env *env, t_token *token)
{
	char	*temp;

	if (token->ttype == TTOKEN_ARGUMENT)
		return (minishell_asterisk(token));
	if (token->ttype == TTOKEN_COMMAND)
		temp = minishell_getpath(env, token->tvalue);
	if (token->ttype == TTOKEN_VARIABLE)
		temp = minishell_getvalue(env, token->tvalue);
	if (!temp)
		return (STATUS_FAILURE); //STATUS_PARSER
	free(token->tvalue);
	token->tvalue = temp;
	return (STATUS_SUCCESS);
}

static t_status    remove_quote(t_token *token)
{
    t_status    status;
    if (token)
    {
        token->tvalue = remove_quotes(token->tvalue);
        if (!token->tvalue)
            return (STATUS_MALLOCERR);
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
        if ((status = remove_quote(token->left)))
            return (status);
        if ((status = remove_quote(token->right)))
            return (status);
        return (STATUS_SUCCESS);
    }
    return (STATUS_SUCCESS);
}
