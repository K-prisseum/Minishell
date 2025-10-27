/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:29:21 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/19 20:38:29 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	count_env_vars(t_env_var *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_to_array(t_env_var *env, t_shell *shell)
{
	char	**env_array;
	int		count;
	int		i;

	count = count_env_vars(env);
	env_array = safe_malloc(sizeof(char *) * (count + 1), shell);
	if (!env_array)
		return (NULL);
	i = 0;
	while (env)
	{
		env_array[i] = ft_strjoin_three(env->name, "=", env->value, shell);
		if (!env_array[i])
		{
			free_env_array(env_array);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	print_env(t_env_var *env)
{
	while (env)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
}
