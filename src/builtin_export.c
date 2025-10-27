/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:02:09 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 17:59:47 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	handle_export_value(char *name, char *equal_pos,
		t_env_var **env, t_shell *shell)
{
	char	*value;

	value = safe_strdup(equal_pos + 1, shell);
	if (!value)
	{
		free(name);
		return (1);
	}
	set_env_var(env, name, value, shell);
	free(value);
	return (0);
}

static int	export_var(char *arg, t_env_var **env, t_shell *shell)
{
	char	*name;
	char	*equal_pos;
	int		ret;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		name = ft_strndup(arg, equal_pos - arg);
	else
		name = safe_strdup(arg, shell);
	if (!name)
		return (1);
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("minishell: export: '", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		free(name);
		return (1);
	}
	ret = 0;
	if (equal_pos)
		ret = handle_export_value(name, equal_pos, env, shell);
	free(name);
	return (ret);
}

int	builtin_export(char **args, t_env_var **env, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!args[1])
		return (print_env(*env), 0);
	while (args[i])
	{
		if (export_var(args[i], env, shell))
			status = 1;
		i++;
	}
	return (status);
}
