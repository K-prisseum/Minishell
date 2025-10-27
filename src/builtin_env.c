/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:00:38 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 17:00:22 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	builtin_env(t_env_var *env)
{
	if (!env)
	{
		ft_putstr_fd("minishell: env: no environment variables\n",
			STDERR_FILENO);
		return (1);
	}
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		env = env->next;
	}
	return (0);
}
