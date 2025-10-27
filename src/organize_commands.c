/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   organize_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:25:18 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/19 23:00:15 by itoudji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
/*
cree une nouvelle commande si current est NULL et l'insere
a la fin de la liste chainee commands
*/

static int	add_command_if_needed(t_command **commands,
	t_command **current, t_shell *shell)
{
	t_command	*tmp;

	if (!*current)
	{
		*current = init_new_command(shell);
		if (!*current)
		{
			print_error("organize_commands", "memory allocation failed");
			free_commands(*commands);
			return (0);
		}
		(*current)->seq_start = 1;
		if (!*commands)
			*commands = *current;
		else
		{
			tmp = *commands;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = *current;
		}
	}
	return (1);
}

void	add_argument_to_command(t_command *command,
	const char *argument, t_shell *shell)
{
	int		argc;
	size_t	old_size;
	size_t	new_size;
	char	**new_argv;

	if (!command || !argument)
		return ;
	argc = array_len(command->argv);
	old_size = sizeof(char *) * (argc + 1);
	new_size = sizeof(char *) * (argc + 2);
	new_argv = safe_realloc(command->argv, old_size, new_size, shell);
	if (!new_argv)
		return ;
	command->argv = new_argv;
	command->argv[argc] = safe_strdup(argument, shell);
	if (!command->argv[argc])
	{
		command->argv[argc] = NULL;
		return ;
	}
	command->argv[argc + 1] = NULL;
}
/*
Gestion du token WORD : duplication et ajout a la commande
*/

int	handle_word_token(t_command *cur, t_token *tok,
	t_shell *shell, t_command *cmds)
{
	char	*arg;

	arg = safe_strdup(tok->value, shell);
	if (!arg)
	{
		print_error("organize_commands",
			"memory allocation failed for argument");
		free_commands(cmds);
		return (0);
	}
	add_argument_to_command(cur, arg, shell);
	free(arg);
	if (tok->value)
	{
		free(tok->value);
		tok->value = NULL;
	}
	return (1);
}

/*
gere le token pipe : marque pipe_out, avance "tokens" et cree
adaptée a une nouvelle commande, renvoie 2 pour un 'continue' immédiat
*/

int	handle_pipe_token(t_token **tokens, t_command **cur,
	t_command **cmds, t_shell *shell)
{
	t_command	*new_cmd;

	(*cur)->pipe_out = 1;
	*tokens = (*tokens)->next;
	if (*tokens)
	{
		new_cmd = init_new_command(shell);
		if (!new_cmd)
		{
			print_error("organize_commands",
				"memory allocation failed after pipe");
			free_commands(*cmds);
			return (0);
		}
		new_cmd->pipe_in = 1;
		new_cmd->seq_start = 0;
		(*cur)->next = new_cmd;
		*cur = new_cmd;
	}
	return (2);
}

/*
fonction principale
- ajoute une nouvelle commande si necessaire
- appelle handle_single_token pour traiter le token
- gere le retour 2 -> continue.
*/

t_command	*organize_commands(t_token *tokens, t_shell *shell)
{
	t_command	*commands;
	t_command	*current;
	int			ret;

	commands = NULL;
	current = NULL;
	while (tokens)
	{
		if (!add_command_if_needed(&commands, &current, shell))
			return (NULL);
		ret = handle_single_token(&tokens, &current, &commands, shell);
		if (ret == 0)
			return (NULL);
		if (ret == 2)
			continue ;
		tokens = tokens->next;
	}
	if (!commands)
	{
		print_error("organize_commands", "no valid commands detected");
		return (NULL);
	}
	return (commands);
}
