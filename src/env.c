/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:28:23 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/19 20:07:51 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_env_var	*env_new_var(char *name, char *value, t_shell *shell)
{
	t_env_var	*new_var;

	new_var = safe_malloc(sizeof(t_env_var), shell);
	if (!new_var)
		return (NULL);
	new_var->name = safe_strdup(name, shell);
	if (!new_var->name)
	{
		free(new_var);
		return (NULL);
	}
	new_var->value = safe_strdup(value, shell);
	if (!new_var->value)
	{
		free(new_var->name);
		free(new_var);
		return (NULL);
	}
	new_var->next = NULL;
	return (new_var);
}

void	env_add_var(t_env_var **env, t_env_var *new_var)
{
	t_env_var	*current;

	if (!*env)
	{
		*env = new_var;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new_var;
}

t_env_var	*env_find_var(t_env_var *env, const char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	free_env(t_env_var *env)
{
	t_env_var	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
}
