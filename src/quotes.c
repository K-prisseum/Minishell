/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itoudji <marvin@42.fr>                      +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:48:34 by itoudji            #+#    #+#            */
/*   Updated: 2024/08/29 01:10:12 by itoudji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*
modifie l'etat des quotes
- c : caractere actuel a analyser
- current_quote : etat actuel des quotes
(NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE)
retourne le nouvel etat des quotes
*/
t_quote_type	set_in_quotes(char c, t_quote_type current_quote)
{
	if (current_quote == NO_QUOTE)
	{
		if (c == '\'')
			return (SINGLE_QUOTE);
		else if (c == '"')
			return (DOUBLE_QUOTE);
	}
	else if ((current_quote == SINGLE_QUOTE && c == '\'')
		|| (current_quote == DOUBLE_QUOTE && c == '"'))
		return (NO_QUOTE);
	return (current_quote);
}

/*
verifie si toutes les quotes dans la chaîne sont fermees
- input: chaine à analyser
retourne 1 si les quotes sont fermees, 0 sinon.
*/
int	check_quotes(const char *input)
{
	t_quote_type	quote_state;
	int				i;

	quote_state = NO_QUOTE;
	i = 0;
	while (input[i])
	{
		quote_state = set_in_quotes(input[i], quote_state);
		i++;
	}
	if (quote_state != NO_QUOTE)
	{
		write(2, "Error: unclosed quotes\n", 23);
		return (0);
	}
	return (1);
}
