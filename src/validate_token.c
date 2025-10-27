/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:26:09 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 16:26:22 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	get_token_category(int type)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_HEREDOC)
		return (0);
	if (type == TOKEN_PIPE)
		return (1);
	return (2);
}

static int	check_next_token(t_token *current)
{
	if (!current->next)
	{
		print_error(NULL, "syntax error near unexpected token `newline'");
		return (0);
	}
	if (current->next->type != TOKEN_WORD)
	{
		if (current->next->type == TOKEN_REDIRECT_IN)
			print_error(NULL, "syntax error near unexpected token `<'");
		else if (current->next->type == TOKEN_REDIRECT_OUT)
			print_error(NULL, "syntax error near unexpected token `>'");
		else if (current->next->type == TOKEN_PIPE)
			print_error(NULL, "syntax error near unexpected token `|'");
		else if (current->next->type == TOKEN_REDIRECT_APPEND)
			print_error(NULL, "syntax error near unexpected token `>>'");
		else if (current->next->type == TOKEN_HEREDOC)
			print_error(NULL, "syntax error near unexpected token `<<'");
		else
			print_error(NULL, "syntax error near unexpected token");
		return (0);
	}
	return (1);
}

static int	handle_pipe(t_token *current, t_token *tokens)
{
	if (current == tokens)
	{
		print_error(NULL, "syntax error near unexpected token `|'");
		return (0);
	}
	if (current->next && current->next->type == TOKEN_PIPE)
	{
		print_error(NULL, "syntax error near unexpected token `|'");
		return (0);
	}
	return (1);
}

static int	check_redir_and_pipe(t_token *cur)
{
	if (!check_next_token(cur))
		return (0);
	if (cur->next && cur->next->type == TOKEN_PIPE)
	{
		print_error(NULL, "syntax error near unexpected token `|'");
		return (0);
	}
	return (1);
}

int	validate_token_sequence(t_token *tokens)
{
	t_token	*cur;
	int		category;

	cur = tokens;
	if (!cur)
		return (0);
	while (cur)
	{
		if (cur->type == TOKEN_UNSUPPORTED)
		{
			print_error(NULL, "unsupported token detected");
			return (0);
		}
		category = get_token_category(cur->type);
		if (category == 0)
		{
			if (!check_redir_and_pipe(cur))
				return (0);
		}
		else if (category == 1 && !handle_pipe(cur, tokens))
			return (0);
		cur = cur->next;
	}
	return (1);
}
