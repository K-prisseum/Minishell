/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:07:56 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 15:08:02 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*resolve_command_path(char *command, t_env_var *env, t_shell *shell);
char	*check_direct_path(char *command);
char	*find_in_path(char *command, t_env_var *env, t_shell *shell);
char	*try_paths(char *command, char **paths, t_shell *shell);

char	*resolve_command_path(char *command, t_env_var *env, t_shell *shell)
{
	if (!command)
		return (NULL);
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
		return (check_direct_path(command));
	return (find_in_path(command, env, shell));
}

char	*check_direct_path(char *command)
{
	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	return (NULL);
}

char	*find_in_path(char *command, t_env_var *env, t_shell *shell)
{
	t_env_var	*path_var;
	char		**paths;
	char		*result;

	path_var = env_find_var(env, "PATH");
	if (!path_var || !path_var->value)
		return (NULL);
	paths = ft_split(path_var->value, ':');
	if (!paths)
		return (NULL);
	result = try_paths(command, paths, shell);
	free_array(paths);
	return (result);
}

char	*try_paths(char *command, char **paths, t_shell *shell)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", command, shell);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
