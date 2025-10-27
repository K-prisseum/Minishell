/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:11:17 by sruzzi            #+#    #+#             */
/*   Updated: 2024/09/04 00:41:12 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static char	*get_input_tty(t_shell *shell)
{
	char	*input;
	char	*prompt;

	prompt = get_prompt(shell);
	if (!prompt)
	{
		perror("Failed to generate prompt");
		return (NULL);
	}
	input = readline(prompt);
	free(prompt);
	return (input);
}

static char	*get_input_pipe(void)
{
	char	*input;
	size_t	len;

	input = get_next_line(STDIN_FILENO);
	if (input)
	{
		len = ft_strlen(input);
		if (len > 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
	}
	return (input);
}

static char	*get_input(t_shell *shell)
{
	if (isatty(STDIN_FILENO))
		return (get_input_tty(shell));
	else
		return (get_input_pipe());
}

char	*read_user_input(t_shell *shell)
{
	char	*input;

	input = get_input(shell);
	if (!input)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		shell->running = 0;
		return (NULL);
	}
	if (*input)
		add_history(input);
	return (input);
}
