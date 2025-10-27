/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:10:48 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 17:10:24 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static char	*parse_escape_sequences(char *str, t_shell *shell)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_str = safe_malloc(ft_strlen(str) + 1, shell);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == 'n')
		{
			new_str[j++] = '\n';
			i += 2;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}

static int	write_echo_arg(char *output, int enable_escape,
	char *orig, t_shell *shell)
{
	if (write(STDOUT_FILENO, output, ft_strlen(output)) == -1)
	{
		print_error("echo", "write error");
		shell->exit_stat = 1;
		if (enable_escape && output != orig)
			free(output);
		return (0);
	}
	if (enable_escape && output != orig)
		free(output);
	return (1);
}

void	print_echo_args(char **args, int i, int enable_escape,
	t_shell *shell)
{
	char	*output;

	while (args[i])
	{
		output = args[i];
		if (enable_escape)
			output = parse_escape_sequences(args[i], shell);
		if (!write_echo_arg(output, enable_escape, args[i], shell))
			return ;
		if (args[i + 1] && write(STDOUT_FILENO, " ", 1) == -1)
		{
			print_error("echo", "write error");
			shell->exit_stat = 1;
			return ;
		}
		i++;
	}
}
