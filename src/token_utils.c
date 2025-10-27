/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:52:06 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 15:52:14 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_token	*token_new(char *value, t_token_type type, t_quote_type quote)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = value;
	new_token->type = type;
	new_token->quote = quote;
	new_token->next = NULL;
	return (new_token);
}

void	token_free(t_token **list)
{
	t_token	*temp;

	if (!list || !*list)
		return ;
	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->value)
			free((*list)->value);
		free(*list);
		*list = temp;
	}
}

void	token_add(t_token **list, t_token *new_token)
{
	t_token	*current;

	if (!list || !new_token)
		return ;
	if (!*list)
	{
		*list = new_token;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token	*get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}
