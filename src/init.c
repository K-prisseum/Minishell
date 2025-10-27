/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:25:47 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 15:26:02 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"


static t_env_var	*process_env_var(char *env_str, t_shell *shell)
{
	t_env_var	*new_var;
	char		*name;
	char		*value;
	char		*eq;

	eq = ft_strchr(env_str, '=');
	if (!eq)
		return (NULL);
	name = ft_strndup(env_str, eq - env_str);
	value = safe_strdup(eq + 1, shell);
	if (!name || !value)
	{
		free(name);
		free(value);
		return (NULL);
	}
	new_var = env_new_var(name, value, shell);
	free(name);
	free(value);
	return (new_var);
}

t_env_var	*copy_env(char **envp, t_shell *shell)
{
	t_env_var	*env_list;
	t_env_var	*new_var;
	int			i;

	env_list = NULL;
	i = 0;
	while (envp && envp[i])
	{
		new_var = process_env_var(envp[i], shell);
		if (new_var)
			env_add_var(&env_list, new_var);
		i++;
	}
	return (env_list);
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp, shell);
	if (!shell->env && envp && envp[0])
	{
		perror("Failed to initialize environment");
		cleanup_shell(shell, NULL, NULL, NULL);
		exit(EXIT_FAILURE);
	}
	shell->exit_stat = 0;
	shell->running = 1;
	shell->pid_count = 0;
	shell->any_int = 0;
	ft_memset(shell->pids, 0, sizeof(shell->pids));
}
