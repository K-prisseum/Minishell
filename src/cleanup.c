/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:36:48 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 16:36:55 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	kill_active_processes(t_shell *shell)
{
	int	i;

	if (!shell || shell->pid_count < 0 || shell->pid_count > MAX_PIDS)
		return ;
	i = 0;
	while (i < shell->pid_count && i < MAX_PIDS)
	{
		if (shell->pids[i] > 0)
		{
			kill(shell->pids[i], SIGKILL);
			shell->pids[i] = 0;
		}
		i++;
	}
	shell->pid_count = 0;
}

static void	cleanup_resources(t_shell *shell, t_command *commands,
		t_token *tokens, char *user_input)
{
	if (g_child_pid > 0)
		g_child_pid = 0;
	if (commands)
	{
		free_commands(commands);
		commands = NULL;
	}
	if (tokens)
	{
		token_free(&tokens);
		tokens = NULL;
	}
	if (user_input)
	{
		free(user_input);
		user_input = NULL;
	}
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
}

void	cleanup_shell(t_shell *shell, t_command *cmds, t_token *tokens,
		char *input)
{
	if (!shell)
		return ;
	kill_active_processes(shell);
	cleanup_resources(shell, cmds, tokens, input);
	ft_memset(shell->pids, 0, sizeof(shell->pids));
	shell->pid_count = 0;
	shell->any_int = 0;
	clear_history();
}

void	free_commands(t_command *commands)
{
	t_command	*temp;

	while (commands)
	{
		temp = commands->next;
		if (commands->argv)
			free_array(commands->argv);
		if (commands->input_file)
			free(commands->input_file);
		if (commands->output_file)
			free(commands->output_file);
		if (commands->heredoc_delimiter)
			free(commands->heredoc_delimiter);
		free(commands);
		commands = temp;
	}
	commands = NULL;
}
