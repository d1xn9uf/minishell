#include "../../inc/parser.h"

static void					fill_tokens(t_token *head, uint32_t size);
static void					get_priority(t_token *head, t_token *token, uint32_t index);
static t_default_priority	default_priority(t_token *token);
static t_token				*parse_tree(t_token *head, uint32_t start, uint32_t end);

t_status	minishell_parser(t_minishell *minishell)
{
	t_token		*head;
	uint32_t	size;
	t_status	status;

	head = minishell->lexer->token;
	size = minishell->lexer->sztoken;
	fill_tokens(head, size);
	minishell->root = parse_tree(head, 0, size);
	if ((status = minishell_translate(minishell->root, minishell->env)))
		return (status);
	return (STATUS_SUCCESS);
}

static void	fill_tokens(t_token *head, uint32_t size)
{
	t_token		*token;
	uint32_t	i;

	token = head;
	i = 0;
	while (i < size)
	{
		get_priority(head, token, i);
		i += 1;
		token = token->next_token;
	}
}

static void		get_priority(t_token *head, t_token *token, uint32_t index)
{
	uint32_t	j;
	t_token		*token_j;
	uint32_t	counter;

	token->priority = default_priority(token);
	j = 0;
	token_j = head;
	counter = 0;
	while (j <= index)
	{
		if (token_j->ttype == TTOKEN_PARENTHESE_OPEN)
			counter += 1;
		if (token_j->ttype == TTOKEN_PARENTHESE_CLOSE)
			counter -= 1;
		j += 1;
		token_j = token_j->next_token;
	}
	if (token->ttype == TTOKEN_PARENTHESE_CLOSE)
		counter += 1;
	token->priority += counter;
}

static t_default_priority	default_priority(t_token *token)
{
	if (token->ttype == TTOKEN_AND_OP || token->ttype == TTOKEN_OR_OP)
		return (PRIORITY_CRITICAL);
	else if (token->ttype == TTOKEN_PIPE)
		return (PRIORITY_HIGH);
	else if (minishell_isred(token))
		return (PRIORITY_MEDIUM);
	else if (token->ttype == TTOKEN_PARENTHESE_OPEN || token->ttype == TTOKEN_PARENTHESE_CLOSE)
		return (PRIORITY_REMOVE);
	return (PRIORITY_IDLE);
}

static t_token	*parse_tree(t_token	*head, uint32_t start, uint32_t end)
{
	uint32_t	i;
	t_token		*token_i;
	uint32_t	prior;
	t_token		*token_p;

	if (end <= start)
		return (NULL);
<<<<<<< HEAD
	prior = 0;
	token_p = head;
	while (prior < start)
	{
		prior += 1;
		token_p = token_p->next_token;
	}
=======
	prior = start;
	token_p = head;
>>>>>>> b1ac43f (saving)
	i = prior + 1;
	token_i = token_p->next_token;
	while (i < end)
	{
		if (token_i->priority < token_p->priority)
		{
			prior = i;
			token_p = token_i;
		}
		i += 1;
		token_i = token_i->next_token;
	}
	token_p->left = parse_tree(head, start, prior);
	token_p->right = parse_tree(token_p->next, prior + 1, end);
	return (token_p);
}
