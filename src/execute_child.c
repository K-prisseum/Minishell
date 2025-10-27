/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:08:32 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 15:50:30 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	execute_child_process(t_shell *shell, t_command *cmd,
			int pipe_fd[2], int prev_pipe_fd);
void	setup_child_pgid(void);
void	execute_child_command(t_command *cmd, t_shell *shell);
void	handle_parent_process(t_shell *shell, t_command *cmd, pid_t pid,
			int *pipe_info);

void	execute_child_process(t_shell *shell, t_command *cmd,
	int pipe_fd[2], int prev_pipe_fd)
{
	setup_child_signals();
	setup_child_pgid();
	setup_child_redirections(cmd, pipe_fd, prev_pipe_fd);
	if (handle_redirections(cmd) < 0)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	execute_child_command(cmd, shell);
}

void	setup_child_pgid(void)
{
	if (g_child_pid > 0)
		setpgid(0, g_child_pid);
	else
		setpgid(0, 0);
}

void	execute_child_command(t_command *cmd, t_shell *shell)
{
	char	*path;

	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd, shell));
	else
	{
		path = resolve_command_path(cmd->argv[0], shell->env, shell);
		if (!path)
		{
			fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
			exit(127);
		}
		execve(path, cmd->argv, env_to_array(shell->env, shell));
		perror("execve");
		free(path);
		exit(127);
	}
}

void	handle_parent_process(t_shell *shell, t_command *cmd, pid_t pid,
		int *pipe_info)
{
	int	pipe_fd[2];
	int	*prev_pipe_fd;

	pipe_fd[0] = pipe_info[0];
	pipe_fd[1] = pipe_info[1];
	prev_pipe_fd = &pipe_info[2];
	if (g_child_pid == 0)
		g_child_pid = pid;
	setpgid(pid, g_child_pid);
	if (shell->pid_count < MAX_PIDS)
		shell->pids[shell->pid_count++] = pid;
	else
		fprintf(stderr, "Warning: Too many processes, PID not tracked\n");
	close_parent_pipes(cmd, pipe_fd, prev_pipe_fd);
}
