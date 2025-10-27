/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:47 by sruzzi            #+#    #+#             */
/*   Updated: 2024/09/04 20:40:11 by itoudji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static long long	handle_overflow(int sign, char c, long long r, int *error)
{
	if (r > (LLONG_MAX / 10)
		|| (r == (LLONG_MAX / 10) && (c - '0') > (LLONG_MAX % 10)))
	{
		*error = 1;
		if (sign == 1)
			return (LLONG_MAX);
		else
			return (LLONG_MIN);
	}
	return (r * 10 + (c - '0'));
}

static long long	ft_atoll(const char *str, int *error)
{
	int			sign;
	long long	result;
	int			i;

	sign = 1;
	result = 0;
	i = 0;
	*error = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		result = handle_overflow(sign, str[i], result, error);
		if (*error)
			return (result);
		i++;
	}
	return (result * sign);
}

static int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == '-' || *str == '+')
		str++;
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] != '\0')
		return (0);
	return (1);
}

static void	exit_with_numeric_error(char *arg, t_shell *shell)
{
	fprintf(stderr, "minishell: exit: %s: numeric argument required\n", arg);
	shell->exit_stat = 255;
	exit(255);
}

int	builtin_exit(char **args, t_shell *shell)
{
	long long	exit_code;
	int			error;
	char		*trimmed;

	if (isatty(STDIN_FILENO))
		printf("exit\n");
	if (!args[1])
		exit(shell->exit_stat);
	trimmed = ft_strtrim(args[1], " \t");
	if (!trimmed || !is_valid_number(trimmed))
	{
		free(trimmed);
		exit_with_numeric_error(args[1], shell);
	}
	exit_code = ft_atoll(trimmed, &error);
	free(trimmed);
	if (error)
		exit_with_numeric_error(args[1], shell);
	if (args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		shell->exit_stat = 1;
		return (1);
	}
	exit(exit_code % 256);
}
