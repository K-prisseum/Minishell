/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:06:55 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/19 17:10:43 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	handle_redirections(t_command *cmd);
int	setup_input_redirection(t_command *cmd);
int	setup_output_redirection(t_command *cmd);

int	handle_redirections(t_command *cmd)
{
	if (cmd->input_file)
	{
		if (setup_input_redirection(cmd) < 0)
			return (-1);
	}
	if (cmd->output_file)
	{
		if (setup_output_redirection(cmd) < 0)
			return (-1);
	}
	return (0);
}

int	setup_input_redirection(t_command *cmd)
{
	int	fd;

	fd = open(cmd->input_file, O_RDONLY);
	if (fd < 0)
	{
		perror("open input_file");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 input_file");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_output_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->append)
		fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open output_file");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 output_file");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
