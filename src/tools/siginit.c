/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   siginit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzary <mzary@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:33:39 by mzary             #+#    #+#             */
/*   Updated: 2025/04/14 11:33:39 by mzary            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tools.h"

t_status	siginit_init(int32_t signum, void (*sighandler)(int32_t))
{
	struct sigaction	sa;

	sa.sa_handler = sighandler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (-1 == sigaction(signum, &sa, NULL))
		return (STATUS_SIGINIT);
	return (STATUS_SUCCESS);
}

void	sigint_hdoc(int32_t signum)
{
	int32_t	fd;

	fd = 3;
	(void)signum;
	while (fd < MAX_FD)
	{
		close(fd);
		fd++;
	}
	exit(STATUS_HDOCSIGINT);
}

void	sigint_handler(int32_t signum)
{
	(void)signum;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_sig_pid == 0)
		rl_redisplay();
}

t_status	minishell_siginit(void)
{
	t_status	status;

	status = siginit_init(SIGINT, sigint_handler);
	if (status)
		return (status);
	signal(SIGQUIT, SIG_IGN);
	return (STATUS_SUCCESS);
}
