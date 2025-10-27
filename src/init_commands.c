/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:27:16 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 16:27:25 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_command	*init_new_command(t_shell *shell)
{
	t_command	*cmd;

	cmd = safe_malloc(sizeof(t_command), shell);
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->pipe_in = -1;
	cmd->pipe_out = 0;
	cmd->seq_start = 0;
	cmd->next = NULL;
	return (cmd);
}
