/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:28:13 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 16:28:20 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	get_end_of_quotes(const char *str, int start)
{
	char	quote;
	int		i;

	quote = str[start];
	i = start + 1;
	while (str[i] && str[i] != quote)
		i++;
	if (!str[i])
		return (-1);
	return (i + 1);
}

static t_quote_type	update_quote_state(t_quote_type quote_state, char c)
{
	if (quote_state == NO_QUOTE)
	{
		if (c == '"')
			return (DOUBLE_QUOTE);
		return (SINGLE_QUOTE);
	}
	if ((quote_state == DOUBLE_QUOTE && c == '"')
		|| (quote_state == SINGLE_QUOTE && c == '\''))
		return (NO_QUOTE);
	return (quote_state);
}

int	get_end_of_argument(const char *str, int start)
{
	int				i;
	t_quote_type	quote_state;

	i = start;
	quote_state = NO_QUOTE;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			quote_state = update_quote_state(quote_state, str[i]);
		else if (str[i] == ' ' && quote_state == NO_QUOTE)
			break ;
		else if ((str[i] == '|' || str[i] == '<' || str[i] == '>')
			&& quote_state == NO_QUOTE)
			break ;
		i++;
	}
	return (i);
}

int	get_next_index(const char *str, int start)
{
	int	i;

	i = start;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (i);
}
