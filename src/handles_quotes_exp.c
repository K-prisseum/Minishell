/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_quotes_exp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:44:54 by sruzzi            #+#    #+#             */
/*   Updated: 2024/09/01 00:10:43 by itoudji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
/*
get_next_token, process_quoted, process_dollar, etc
jlaisse en l’état si ça te convient en veillant juste à ce que ce qui retourne soit juste 1 seul malloc
*/

static char	*process_quoted(const char *input, int *i, t_shell *shell)
{
	t_quote_type	quote_state;
	int				start;
	char			*sub;
	char			*tmp;

	quote_state = set_in_quotes(input[*i], NO_QUOTE);
	(*i)++;
	start = *i;
	while (input[*i] && set_in_quotes(input[*i], quote_state) == quote_state)
		(*i)++;
	if (!input[*i])
		return (NULL);
	sub = ft_substr(input, start, (*i) - start);
	if (!sub)
		return (NULL);
	if (quote_state == DOUBLE_QUOTE)
	{
		tmp = sub;
		sub = replace_env_variables(sub, shell);
		free(tmp);
		if (!sub)
			return (NULL);
	}
	(*i)++;
	return (sub);
}

static char	*process_dollar(const char *input, int *i, t_shell *shell)
{
	int		start;
	char	*sub;
	char	*expanded;

	start = *i;
	while (input[++(*i)] && (ft_isalnum(input[*i]) || input[*i] == '_'
			|| input[*i] == '?'));
	sub = ft_substr(input, start, (*i) - start);
	if (!sub)
		return (NULL);
	expanded = replace_env_variables(sub, shell);
	free(sub);
	if (!expanded)
		return (NULL);
	return (expanded);
}

static char	*get_next_token(const char *input, int *i, t_shell *shell)
{
	char	*token;
	char	temp[2];

	if (input[*i] == '\'' || input[*i] == '"')
		token = process_quoted(input, i, shell);
	else if (input[*i] == '$' && (ft_isalnum(input[*i + 1])
			|| input[*i + 1] == '_' || input[*i + 1] == '?'))
		token = process_dollar(input, i, shell);
	else
	{
		temp[0] = input[*i];
		temp[1] = '\0';
		token = safe_strdup(temp, shell);
		if (!token)
			return (NULL);
		(*i)++;
	}
	return (token);
}

char	*process_quotes_and_expansion(const char *input, t_shell *shell)
{
	char	*result;
	char	*token;
	char	*old;
	int		i;

	result = safe_strdup("", shell);
	if (!result)
		return (NULL);
	i = 0;
	while (input[i])
	{
		token = get_next_token(input, &i, shell);
		if (!token)
		{
			free(result);
			return (NULL);
		}
		old = result;
		result = ft_strjoin_multiple(shell, 2, result, token);
		free(old);
		free(token);
		if (!result)
			return (NULL);
	}
	return (result);
}
