/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:52:21 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 16:10:26 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	process_single_redirect(const char *input, int i,
		t_token **tokens, t_shell *shell)
{
	if (input[i] == '<')
		token_add(tokens, token_new(safe_substr(input, i, 1, shell),
				TOKEN_REDIRECT_IN, NO_QUOTE));
	else
		token_add(tokens, token_new(safe_substr(input, i, 1, shell),
				TOKEN_REDIRECT_OUT, NO_QUOTE));
	i++;
	return (i);
}

static int	process_double_redirect(const char *input, int i,
	t_token **tokens, t_shell *shell)
{
	int		type;
	char	*token_str;
	t_token	*new_token;

	if (input[i] == '<')
		type = TOKEN_HEREDOC;
	else
		type = TOKEN_REDIRECT_APPEND;
	token_str = safe_substr(input, i, 2, shell);
	if (!token_str)
		return (-1);
	new_token = token_new(token_str, type, NO_QUOTE);
	if (!new_token)
	{
		free(token_str);
		return (-1);
	}
	token_add(tokens, new_token);
	i += 2;
	if (type == TOKEN_HEREDOC)
		i = process_heredoc_delim(input, i, tokens, shell);
	return (i);
}

static int	handle_token(const char *input, int i,
		t_token **tokens, t_shell *shell)
{
	int	result;

	result = process_special_op(input, i, tokens, shell);
	if (result != i)
		return (result);
	if (input[i] == '<' || input[i] == '>')
	{
		if (input[i + 1] == input[i])
			return (process_double_redirect(input, i, tokens, shell));
		else
			return (process_single_redirect(input, i, tokens, shell));
	}
	return (process_word(input, i, tokens, shell));
}

t_token	*tokenize_input(const char *input, t_shell *shell)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	if (!check_quotes(input))
		return (NULL);
	while (input[i])
	{
		i = get_next_index(input, i);
		if (!input[i])
			break ;
		i = handle_token(input, i, &tokens, shell);
		if (i == -1)
		{
			token_free(&tokens);
			return (NULL);
		}
	}
	return (tokens);
}
