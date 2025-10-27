/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:05:37 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/28 16:20:50 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	detect_delimiter_and_strip_quotes(char **del_ptr)
{
	char	*del;
	char	*tmp;
	size_t	len;

	del = *del_ptr;
	len = ft_strlen(del);
	if (len >= 2 && del[0] == '\'' && del[len - 1] == '\'')
	{
		tmp = ft_substr(del, 1, len - 2);
		if (!tmp)
			return (1);
		free(del);
		*del_ptr = tmp;
		return (0);
	}
	else if (len >= 2 && del[0] == '"' && del[len - 1] == '"')
	{
		tmp = ft_substr(del, 1, len - 2);
		if (!tmp)
			return (1);
		free(del);
		*del_ptr = tmp;
		return (0);
	}
	return (1);
}

int	setup_heredoc_process(t_command *cmd, int pipe_fd[2],
	int expansion, t_shell *shell)
{
	pid_t	pid;

	g_child_pid = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
		heredoc_child(cmd, shell, pipe_fd, expansion);
	g_child_pid = pid;
	return (pid);
}

static int	check_heredoc_signal(int status, int fd, t_shell *shell)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(fd);
		shell->exit_stat = 130;
		write(STDOUT_FILENO, "\n", 1);
		return (-1);
	}
	return (0);
}

static int	check_cmd_validity(t_command *cmd, int fd)
{
	if (!cmd->argv || !cmd->argv[0])
	{
		close(fd);
		return (-1);
	}
	return (0);
}

int	heredoc_parent(t_command *cmd, t_shell *shell, int *pipe_fd, pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	g_child_pid = 0;
	if (check_heredoc_signal(status, pipe_fd[0], shell) == -1)
		return (-1);
	if (check_cmd_validity(cmd, pipe_fd[0]) == -1)
		return (-1);
	cmd->pipe_in = pipe_fd[0];
	return (0);
}
