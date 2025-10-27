/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:59:14 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 14:59:51 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	handle_cd_oldpwd(t_shell *shell, int *free_path)
{
	char	*path;
	int		ret;

	path = get_env_value(shell->env, "OLDPWD");
	*free_path = 1;
	if (!path || !*path)
	{
		print_error("cd", "OLDPWD not set");
		if (path)
			free(path);
		return (1);
	}
	ret = change_directory(path, shell);
	if (*free_path)
		free(path);
	return (ret);
}

int	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	*path;
	int		free_path;
	int		ret;

	free_path = 0;
	if (!cmd->argv[1])
	{
		path = get_env_value(shell->env, "HOME");
		free_path = 1;
		if (!path || !*path)
		{
			print_error("cd", "HOME not set");
			if (path)
				free(path);
			return (1);
		}
	}
	else if (ft_strcmp(cmd->argv[1], "-") == 0)
		return (handle_cd_oldpwd(shell, &free_path));
	else
		path = cmd->argv[1];
	ret = change_directory(path, shell);
	if (free_path)
		free(path);
	return (ret);
}
