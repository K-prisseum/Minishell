/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:23:10 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 15:23:37 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static	t_token	*prepare_tokens(char *input, t_shell *shell)
{
	t_token	*tokens;
	int		result;

	tokens = tokenize_input(input, shell);
	if (!tokens)
		return (NULL);
	result = validate_token_sequence(tokens);
	if (!result)
	{
		token_free(&tokens);
		return (NULL);
	}
	return (tokens);
}

static	t_command	*prepare_commands(t_token *tokens, t_shell *shell,
	char *input)
{
	t_command	*commands;

	commands = organize_commands(tokens, shell);
	token_free(&tokens);
	if (!commands || validate_commands(commands) < 0)
	{
		if (commands)
			free_commands(commands);
		free(input);
		return (NULL);
	}
	return (commands);
}

void	process_input(t_shell *shell)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;

	input = read_user_input(shell);
	if (!input || !shell->running)
		return ;
	tokens = prepare_tokens(input, shell);
	if (!tokens)
	{
		free(input);
		return ;
	}
	commands = prepare_commands(tokens, shell, input);
	if (!commands)
		return ;
	execute_commands(shell, commands);
	free_commands(commands);
	free(input);
}
