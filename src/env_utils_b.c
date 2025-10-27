/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:39:50 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/19 21:20:04 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*get_env_value(t_env_var *env, const char *key)
{
	if (!key || !env)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->name, key) == 0)
		{
			return (ft_strdup(env->value));
		}
		env = env->next;
	}
	return (NULL);
}

void	set_env_var(t_env_var **env, char *name, char *value, t_shell *shell)
{
	t_env_var	*var;

	var = env_find_var(*env, name);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
		if (!var->value)
			return ;
	}
	else
		env_add_var(env, env_new_var(name, value, shell));
}

void	unset_env_var(t_env_var **env, char *name)
{
	t_env_var	*current;
	t_env_var	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
