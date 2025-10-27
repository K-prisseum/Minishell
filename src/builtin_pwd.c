/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:12:01 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 15:20:08 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	builtin_pwd(t_shell *shell)
{
	char	buffer[1024];
	char	*env_pwd;

	if (getcwd(buffer, sizeof(buffer)))
	{
		ft_putstr_fd(buffer, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		shell->exit_stat = 0;
		return (0);
	}
	env_pwd = get_env_value(shell->env, "PWD");
	if (env_pwd)
	{
		ft_putstr_fd(env_pwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		free(env_pwd);
		shell->exit_stat = 0;
		return (0);
	}
	ft_putstr_fd(".\n", STDOUT_FILENO);
	shell->exit_stat = 1;
	return (1);
}
