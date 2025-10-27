/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:06:32 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 15:06:38 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	execute_builtin_command(t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd(shell));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd->argv, &shell->env, shell));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd->argv, &shell->env));
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd, shell));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(shell->env));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd->argv, shell));
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
	{
		builtin_echo(cmd->argv, shell);
		return (0);
	}
	return (1);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	int	ret;

	if (handle_redirections(cmd) < 0)
	{
		shell->exit_stat = 1;
		return (1);
	}
	ret = execute_builtin_command(cmd, shell);
	shell->exit_stat = ret;
	return (ret);
}
