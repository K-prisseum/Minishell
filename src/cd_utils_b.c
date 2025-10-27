/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_b.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:09:14 by sruzzi            #+#    #+#             */
/*   Updated: 2024/09/01 22:25:50 by itoudji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
static int	check_path_and_chdir(const char *path, char *new_logical_path,
			t_shell *shell)
{
	if (access(path, F_OK) != 0)
	{
		print_error("cd", path);
		print_error(NULL, "No such file or directory");
		free(new_logical_path);
		shell->exit_stat = 1;
		return (1);
	}
	if (chdir(path) == -1)
	{
		print_error("cd", "chdir failed");
		free(new_logical_path);
		shell->exit_stat = 1;
		return (1);
	}
	return (0);
}

/*
met à jour PWD dans l'environnement
utilise getcwd() si possible, sinon utilise le chemin logique
*/
static int	update_pwd(char *new_logical_path, t_shell *shell)
{
	char	buffer[1024];

	if (!getcwd(buffer, sizeof(buffer)))
	{
		print_error("cd", "getcwd failed (directory might be deleted)");
		update_env_vars(&shell->env, new_logical_path, shell);
		free(new_logical_path);
		shell->exit_stat = 0;
		return (0);
	}
	update_env_vars(&shell->env, buffer, shell);
	free(new_logical_path);
	shell->exit_stat = 0;
	return (0);
}

static void	init_cd_vars(t_shell *shell, char **old_pwd,
	char **old_pwd_temp)
{
	*old_pwd_temp = get_env_value(shell->env, "PWD");
	if (!*old_pwd_temp)
		*old_pwd = "";
	else
		*old_pwd = *old_pwd_temp;
}

/*
change_directory:
- build le chemin logique
- appelle chdir()
- met à jour PWD dans l'environnement
*/
int	change_directory(const char *path, t_shell *shell)
{
	char	*old_pwd_temp;
	char	*old_pwd;
	char	*new_logical_path;

	init_cd_vars(shell, &old_pwd, &old_pwd_temp);
	new_logical_path = build_logical_path(old_pwd, path);
	if (old_pwd_temp)
		free(old_pwd_temp);
	if (!new_logical_path)
	{
		print_error("cd", "malloc failed");
		shell->exit_stat = 1;
		return (1);
	}
	if (check_path_and_chdir(path, new_logical_path, shell) != 0)
		return (1);
	return (update_pwd(new_logical_path, shell));
}
