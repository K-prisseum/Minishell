/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:07:37 by sruzzi            #+#    #+#             */
/*   Updated: 2025/09/01 00:34:04 by itoudji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	handle_quotes(const char *input, int i, t_quote_type *quote_type)
{
	if (input[i] == '\'')
	{
		*quote_type = SINGLE_QUOTE;
		return (get_end_of_quotes(input, i));
	}
	else if (input[i] == '"')
	{
		*quote_type = DOUBLE_QUOTE;
		return (get_end_of_quotes(input, i));
	}
	else
	{
		*quote_type = NO_QUOTE;
		return (get_end_of_argument(input, i));
	}
}

static int	merge_tokens(t_token *last_token, char *processed_value)
{
	char	*new_value;

	new_value = ft_strjoin(last_token->value, processed_value);
	if (!new_value)
	{
		free(processed_value);
		return (-1);
	}
	free(last_token->value);
	last_token->value = new_value;
	free(processed_value);
	return (0);
}

static char	*extract_word(const char *input, int *i, t_shell *shell,
		t_quote_type *quote_type)
{
	int		start;
	char	*tmp_sub;
	char	*processed_value;

	start = *i;
	*i = handle_quotes(input, *i, quote_type);
	if (*i == -1)
		return (NULL);
	tmp_sub = safe_substr(input, start, *i - start, shell);
	if (!tmp_sub)
		return (NULL);
	processed_value = process_quotes_and_expansion(tmp_sub, shell);
	free(tmp_sub);
	return (processed_value);
}

int	process_word(const char *input, int i, t_token **tokens, t_shell *shell)
{
	char			*processed_value;
	t_token			*last_token;
	t_quote_type	quote_type;

	processed_value = extract_word(input, &i, shell, &quote_type);
	if (!processed_value)
		return (-1);
	last_token = get_last_token(*tokens);
	if (last_token && last_token->type == TOKEN_WORD
		&& last_token->quote != NO_QUOTE)
	{
		if (merge_tokens(last_token, processed_value) == -1)
			return (-1);
	}
	else
	{
		token_add(tokens, token_new(processed_value, TOKEN_WORD, quote_type));
	}
	return (i);
}
