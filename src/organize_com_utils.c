/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   organize_com_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:25:00 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/19 23:38:11 by itoudji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*
affecte le fichier d'entree a la commande, avec check memoire
*/
static int	handle_redirect_in_token(t_token **tokens, t_command *cur,
	t_shell *shell, t_command *cmds)
{
	if (cur->input_file)
		free(cur->input_file);
	cur->input_file = safe_strdup((*tokens)->next->value, shell);
	if (!cur->input_file)
	{
		print_error("organize_commands",
			"memory allocation failed for input_file");
		free_commands(cmds);
		return (0);
	}
	*tokens = (*tokens)->next;
	return (1);
}

/*
affecte le fichier de sortie à la commande, avec check memoire
*/
static int	handle_redirect_out_token(t_token **tokens, t_command *cur,
	t_shell *shell, t_command *cmds)
{
	if (cur->output_file)
		free(cur->output_file);
	cur->output_file = safe_strdup((*tokens)->next->value, shell);
	if (!cur->output_file)
	{
		print_error("organize_commands",
			"memory allocation failed for output_file");
		free_commands(cmds);
		return (0);
	}
	cur->append = ((*tokens)->type == TOKEN_REDIRECT_APPEND);
	*tokens = (*tokens)->next;
	return (1);
}

/*
affecte le delimiteur heredoc a la commande, avec check memoire
*/
static int	handle_heredoc_token(t_token **tokens, t_command *cur,
	t_shell *shell, t_command *cmds)
{
	if (cur->heredoc_delimiter)
		free(cur->heredoc_delimiter);
	cur->heredoc_delimiter = safe_strdup((*tokens)->next->value, shell);
	if (!cur->heredoc_delimiter)
	{
		print_error("organize_commands",
			"memory allocation failed for heredoc_delimiter");
		free_commands(cmds);
		return (0);
	}
	cur->heredoc = 1;
	*tokens = (*tokens)->next;
	return (1);
}
/*
gère le token "";""
- separe les commandes
- retourne 2 pour forcer un "continue" dans "organize_commands"
*/

static int	handle_semicolon_token(t_token **tokens, t_command **cur)
{
	*cur = NULL;
	*tokens = (*tokens)->next;
	return (2);
}

/*
route vers la fonction adaptee selon le type de token
retourne 0 en cas d'erreur, 2 pour un pipe, 1 sinon
*/

int	handle_single_token(t_token **tokens, t_command **cur,
	t_command **cmds, t_shell *shell)
{
	if ((*tokens)->type == TOKEN_WORD)
		return (handle_word_token(*cur, *tokens, shell, *cmds));
	else if ((*tokens)->type == TOKEN_PIPE)
		return (handle_pipe_token(tokens, cur, cmds, shell));
	else if ((*tokens)->type == TOKEN_REDIRECT_IN)
		return (handle_redirect_in_token(tokens, *cur, shell, *cmds));
	else if ((*tokens)->type == TOKEN_REDIRECT_OUT
		|| (*tokens)->type == TOKEN_REDIRECT_APPEND)
		return (handle_redirect_out_token(tokens, *cur, shell, *cmds));
	else if ((*tokens)->type == TOKEN_HEREDOC)
		return (handle_heredoc_token(tokens, *cur, shell, *cmds));
	else if ((*tokens)->type == TOKEN_SEMICOLON)
		return (handle_semicolon_token(tokens, cur));
	return (1);
}
