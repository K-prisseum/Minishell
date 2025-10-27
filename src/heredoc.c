/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:05:51 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 16:21:07 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	read_heredoc_line(t_command *cmd, int fd, char **line)
{
	*line = readline("heredoc> ");
	if (!*line)
	{
		fprintf(stderr, "bash: warning: here-document delimited by end-of-file "
			"(wanted `%s')\n", cmd->heredoc_delimiter);
		close(fd);
		exit(0);
	}
	if (ft_strcmp(*line, cmd->heredoc_delimiter) == 0)
	{
		free(*line);
		return (1);
	}
	return (0);
}

static char	*expand_heredoc_line(char *line, t_shell *shell,
	int fd, int expansion)
{
	char	*expanded;

	if (!expansion)
		return (line);
	expanded = replace_env_variables(line, shell);
	free(line);
	if (!expanded)
	{
		close(fd);
		exit(1);
	}
	return (expanded);
}

static void	write_heredoc_line(char *line, int fd)
{
	if (write(fd, line, ft_strlen(line)) < 0 || write(fd, "\n", 1) < 0)
	{
		free(line);
		close(fd);
		exit(1);
	}
	free(line);
}

void	heredoc_child(t_command *cmd, t_shell *shell,
	int *pipe_fd, int expansion)
{
	char	*line;
	int		done;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
	done = 0;
	while (!done)
	{
		done = read_heredoc_line(cmd, pipe_fd[1], &line);
		if (!done)
		{
			line = expand_heredoc_line(line, shell, pipe_fd[1], expansion);
			write_heredoc_line(line, pipe_fd[1]);
		}
	}
	close(pipe_fd[1]);
	exit(0);
}

int	handle_heredoc(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	int		expansion;
	pid_t	pid;
	int		result;

	if (!cmd->heredoc_delimiter)
		return (0);
	expansion = detect_delimiter_and_strip_quotes(&cmd->heredoc_delimiter);
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	pid = setup_heredoc_process(cmd, pipe_fd, expansion, shell);
	if (pid == -1)
		return (-1);
	result = heredoc_parent(cmd, shell, pipe_fd, pid);
	return (result);
}
