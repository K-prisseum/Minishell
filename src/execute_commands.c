/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:18:36 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/19 19:58:22 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	execute_commands(t_shell *shell, t_command *commands);
void	execute_single_command(t_shell *shell, t_command *cmd,
			int *prev_pipe_fd);
void	handle_critical_builtin(t_shell *shell, t_command *cmd, int pipe_fd[2],
			int *prev_pipe_fd);
void	handle_forked_command(t_shell *shell, t_command *cmd, int pipe_fd[2],
			int *prev_pipe_fd);
void	wait_for_children(t_shell *shell);

void	execute_commands(t_shell *shell, t_command *commands)
{
	t_command	*cmd;
	t_command	*group_start;
	int			prev_pipe_fd;

	cmd = commands;
	while (cmd)
	{
		prev_pipe_fd = -1;
		shell->pid_count = 0;
		group_start = cmd;
		while (cmd && (cmd == group_start || cmd->seq_start == 0))
		{
			execute_single_command(shell, cmd, &prev_pipe_fd);
			cmd = cmd->next;
		}
		wait_for_children(shell);
		g_child_pid = 0;
		shell->pid_count = 0;
	}
}

void	execute_single_command(t_shell *shell, t_command *cmd,
	int *prev_pipe_fd)
{
	int	pipe_fd[2];

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (cmd->heredoc_delimiter)
	{
		if (handle_heredoc(cmd, shell) < 0)
			return ;
	}
	if (cmd->pipe_out)
	{
		if (pipe(pipe_fd) < 0)
		{
			perror("pipe");
			return ;
		}
	}
	if (cmd->argv && cmd->argv[0] && is_critical_builtin(cmd->argv[0]))
		handle_critical_builtin(shell, cmd, pipe_fd, prev_pipe_fd);
	else
		handle_forked_command(shell, cmd, pipe_fd, prev_pipe_fd);
}

void	handle_critical_builtin(t_shell *shell, t_command *cmd,
							int pipe_fd[2], int *prev_pipe_fd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	setup_parent_redirections(cmd, pipe_fd, *prev_pipe_fd);
	if (handle_redirections(cmd) < 0)
	{
		restore_parent_redirections(saved_stdin, saved_stdout);
		return ;
	}
	shell->exit_stat = execute_builtin(cmd, shell);
	restore_parent_redirections(saved_stdin, saved_stdout);
	update_prev_pipe_fd(cmd, pipe_fd, prev_pipe_fd);
}

void	handle_forked_command(t_shell *shell, t_command *cmd,
						int pipe_fd[2], int *prev_pipe_fd)
{
	pid_t	pid;
	int		pipe_info[3];

	pid = fork();
	if (pid == 0)
		execute_child_process(shell, cmd, pipe_fd, *prev_pipe_fd);
	else if (pid < 0)
	{
		perror("fork");
		return ;
	}
	else
	{
		pipe_info[0] = pipe_fd[0];
		pipe_info[1] = pipe_fd[1];
		pipe_info[2] = *prev_pipe_fd;
		handle_parent_process(shell, cmd, pid, pipe_info);
	}
}

void	wait_for_children(t_shell *shell)
{
	int	status;

	shell->any_int = 0;
	wait_and_check_status(shell, &status);
	update_exit_status(shell, status);
	if (shell->any_int)
		write(STDOUT_FILENO, "\n", 1);
}
