/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:08:13 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/19 19:50:23 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	setup_parent_redirections(t_command *cmd, int pipe_fd[2],
			int prev_pipe_fd);
void	restore_parent_redirections(int saved_stdin, int saved_stdout);
void	update_prev_pipe_fd(t_command *cmd, int pipe_fd[2],
			int *prev_pipe_fd);
void	setup_child_redirections(t_command *cmd, int pipe_fd[2],
			int prev_pipe_fd);
void	close_parent_pipes(t_command *cmd, int pipe_fd[2], int *prev_pipe_fd);

void	setup_parent_redirections(t_command *cmd, int pipe_fd[2],
	int prev_pipe_fd)
{
	if (prev_pipe_fd != -1)
	{
		dup2(prev_pipe_fd, STDIN_FILENO);
		close(prev_pipe_fd);
	}
	else if (cmd->pipe_in != -1)
	{
		dup2(cmd->pipe_in, STDIN_FILENO);
		close(cmd->pipe_in);
	}
	if (cmd->pipe_out)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	restore_parent_redirections(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	update_prev_pipe_fd(t_command *cmd, int pipe_fd[2], int *prev_pipe_fd)
{
	if (cmd->pipe_out)
	{
		close(pipe_fd[1]);
		*prev_pipe_fd = pipe_fd[0];
	}
	else
		*prev_pipe_fd = -1;
}

void	setup_child_redirections(t_command *cmd, int pipe_fd[2],
	int prev_pipe_fd)
{
	if (prev_pipe_fd != -1)
	{
		dup2(prev_pipe_fd, STDIN_FILENO);
		close(prev_pipe_fd);
	}
	else if (cmd->pipe_in != -1)
	{
		dup2(cmd->pipe_in, STDIN_FILENO);
		close(cmd->pipe_in);
	}
	if (cmd->pipe_out)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	close_parent_pipes(t_command *cmd, int pipe_fd[2], int *prev_pipe_fd)
{
	if (*prev_pipe_fd != -1)
		close(*prev_pipe_fd);
	if (cmd->pipe_out)
	{
		close(pipe_fd[1]);
		*prev_pipe_fd = pipe_fd[0];
	}
	else
		*prev_pipe_fd = -1;
}
