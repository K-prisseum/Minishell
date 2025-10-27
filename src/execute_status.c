/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:13:05 by sruzzi            #+#    #+#             */
/*   Updated: 2024/09/04 01:12:01 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	wait_and_check_status(t_shell *shell, int *status);
void	update_exit_status(t_shell *shell, int status);
//void	display_signal_messages(t_shell *shell, t_command *commands);
//void	display_stopped_job(t_shell *shell, t_command *commands);

void	wait_and_check_status(t_shell *shell, int *status)
{
	int	i;

	i = 0;
	while (i < shell->pid_count)
	{
		waitpid(shell->pids[i], status, 0);
		if (WIFSIGNALED(*status))
		{
			if (WTERMSIG(*status) == SIGINT)
				shell->any_int = 1;
		}
		i++;
	}
}

void	update_exit_status(t_shell *shell, int status)
{
	if (shell->pid_count > 0)
	{
		if (WIFEXITED(status))
			shell->exit_stat = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				shell->exit_stat = 130;
		}
	}
}
