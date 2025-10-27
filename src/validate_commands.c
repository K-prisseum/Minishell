/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:12:23 by sruzzi            #+#    #+#             */
/*   Updated: 2024/09/04 22:17:54 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	validate_empty_command(t_command *cmd)
{
	if (!cmd->argv || !cmd->argv[0])
	{
		if (cmd->heredoc_delimiter || cmd->input_file || cmd->output_file)
			return (1);
		print_error(NULL, "syntax error: unexpected empty command");
		return (0);
	}
	return (1);
}

static int	validate_input_file(t_command *cmd)
{
	if (cmd->input_file && access(cmd->input_file, F_OK) != 0)
	{
		print_error(cmd->input_file, "No such file or directory");
		return (0);
	}
	return (1);
}

static int	validate_pipe_in(t_command *cmd)
{
	if (cmd->pipe_in && (!cmd->argv || !cmd->argv[0]))
	{
		if (cmd->heredoc_delimiter || cmd->input_file || cmd->output_file)
			return (1);
		print_error(NULL, "syntax error near unexpected token `|'");
		return (0);
	}
	return (1);
}

int	validate_commands(t_command *commands)
{
	t_command	*cmd;

	cmd = commands;
	while (cmd)
	{
		if (!validate_empty_command(cmd))
			return (-1);
		if (!validate_input_file(cmd))
			return (-1);
		if (!validate_pipe_in(cmd))
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}
