/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_a.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:10:14 by sruzzi            #+#    #+#             */
/*   Updated: 2024/09/01 22:40:39 by itoudji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	copy_dir_to_path(char *new_path, const char *old_pwd)
{
	int	i;

	i = 0;
	while (old_pwd && old_pwd[i])
	{
		new_path[i] = old_pwd[i];
		i++;
	}
	if (i > 0 && new_path[i - 1] != '/')
	{
		new_path[i] = '/';
		i++;
	}
	return (i);
}

static char	*copy_arg_to_path(char *new_path, const char *arg, int i)
{
	int	j;

	j = 0;
	while (arg[j])
	{
		new_path[i] = arg[j];
		i++;
		j++;
	}
	new_path[i] = '\0';
	return (new_path);
}

char	*build_logical_path(const char *old_pwd, const char *arg)
{
	char	*new_path;
	int		len_old;
	int		len_arg;
	int		i;

	if (!arg)
		return (NULL);
	if (arg[0] == '/')
		return (ft_strdup(arg));
	len_old = ft_strlen(old_pwd);
	len_arg = ft_strlen(arg);
	new_path = malloc(sizeof(char) * (len_old + 1 + len_arg + 1));
	if (!new_path)
		return (NULL);
	i = copy_dir_to_path(new_path, old_pwd);
	return (copy_arg_to_path(new_path, arg, i));
}

/*
met à jour OLDPWD avec l'ancien PWD, ensuite PWD avec new_logical_path
*/
void	update_env_vars(t_env_var **env, const char *new_logical_path,
	t_shell *shell)
{
	char	*old_pwd;

	old_pwd = get_env_value(*env, "PWD");
	if (old_pwd)
	{
		set_env_var(env, "OLDPWD", old_pwd, shell);
		free(old_pwd);
	}
	if (new_logical_path && *new_logical_path)
		set_env_var(env, "PWD", (char *)new_logical_path, shell);
}
