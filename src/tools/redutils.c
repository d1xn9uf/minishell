#include "../../inc/tools.h"

bool		minishell_isred(t_root *node)
{
	if (!node)
		return(false);
	return (
		node->ttype == TTOKEN_HEREDOC ||
		node->ttype == TTOKEN_OUTPUT ||
		node->ttype == TTOKEN_APPEND ||
		node->ttype == TTOKEN_INPUT
	);
}
