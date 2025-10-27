/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:00:12 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 16:20:27 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
/* Vérifie si un argument est une option `-n`, `-e` ou `-E` */
static int	is_valid_echo_option(const char *arg, char opt)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != opt)
		return (0);
	i = 1;
	while (arg[++i])
		if (arg[i] != opt)
			return (0);
	return (1);
}

/* Traite les options de la commande echo */
static int	process_echo_options(char **args, int *newline, int *enable_escape)
{
	int	i;

	i = 1;
	*newline = 1;
	*enable_escape = 0;
	while (args[i])
	{
		if (is_valid_echo_option(args[i], 'n'))
			*newline = 0;
		else if (is_valid_echo_option(args[i], 'e'))
			*enable_escape = 1;
		else if (is_valid_echo_option(args[i], 'E'))
			*enable_escape = 0;
		else
			break ;
		i++;
	}
	return (i);
}

/* Exécution de `echo` */
void	builtin_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;
	int	enable_escape;

	i = process_echo_options(args, &newline, &enable_escape);
	print_echo_args(args, i, enable_escape, shell);
	if (newline && write(STDOUT_FILENO, "\n", 1) == -1)
	{
		print_error("echo", "write error");
		shell->exit_stat = 1;
		return ;
	}
	shell->exit_stat = 0;
}
