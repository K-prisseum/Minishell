/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:53:48 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 16:23:54 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	extract_heredoc_delimiter(const char *input, int i,
		char **token_str, t_shell *shell)
{
	int	start;

	i = get_next_index(input, i);
	if (!input[i] || input[i] == '|')
		return (i);
	start = i;
	if (input[i] == '\'' || input[i] == '"')
		i = get_end_of_quotes(input, i);
	else
		i = get_end_of_argument(input, i);
	if (i == -1)
		return (-1);
	*token_str = safe_substr(input, start, i - start, shell);
	if (!*token_str)
		return (-1);
	return (i);
}

static int	process_semicolon(int i, t_token **tokens)
{
	char	*token_value;
	t_token	*new_token;

	token_value = ft_strdup(";");
	if (!token_value)
		return (-1);
	new_token = token_new(token_value, TOKEN_SEMICOLON, NO_QUOTE);
	if (!new_token)
	{
		free(token_value);
		return (-1);
	}
	token_add(tokens, new_token);
	return (i + 1);
}

int	process_special_op(const char *input, int i,
		t_token **tokens, t_shell *shell)
{
	if ((input[i] == '&' && input[i + 1] == '&')
		|| (input[i] == '|' && input[i + 1] == '|'))
	{
		token_add(tokens, token_new(safe_substr(input, i, 2, shell),
				TOKEN_UNSUPPORTED, NO_QUOTE));
		return (i + 2);
	}
	if (input[i] == '|')
	{
		token_add(tokens, token_new(safe_substr(input, i, 1, shell),
				TOKEN_PIPE, NO_QUOTE));
		return (i + 1);
	}
	if (input[i] == ';')
		return (process_semicolon(i, tokens));
	return (i);
}

int	process_heredoc_delim(const char *input, int i,
	t_token **tokens, t_shell *shell)
{
	char	*token_str;
	t_token	*new_token;

	i = extract_heredoc_delimiter(input, i, &token_str, shell);
	if (i == -1 || (!input[i] && !token_str))
		return (i);
	if (token_str)
	{
		new_token = token_new(token_str, TOKEN_WORD, NO_QUOTE);
		if (!new_token)
		{
			free(token_str);
			return (-1);
		}
		token_add(tokens, new_token);
	}
	return (i);
}
