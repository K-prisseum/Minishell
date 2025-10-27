/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <you> <you@42.fr>                          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 00:00:00 by <you>             #+#    #+#             */
/*   Updated: 2024/09/10 20:57:30 by <itoudji>        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# define COLOR_RESET	"\033[0m"
# define GREEN			"\033[32m"
# define RED			"\033[31m"
# define LRED			"\033[38;5;203m"
# define YELLOW			"\033[33m"
# define ORANGE			"\033[38;5;214m"
# define MAX_PIDS		1024

extern volatile sig_atomic_t	g_child_pid;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SEMICOLON,
	TOKEN_UNSUPPORTED
}	t_token_type;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_quote_type	quote;
	struct s_token	*next;
}				t_token;

typedef struct s_command
{
	char				**argv;
	char				*input_file;
	char				*output_file;
	int					append;
	int					heredoc;
	char				*heredoc_delimiter;
	int					pipe_in;
	int					pipe_out;
	int					seq_start;
	struct s_command	*next;
}	t_command;

typedef struct s_env_var
{
	char				*name;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_shell
{
	t_env_var	*env;
	int			exit_stat;
	int			running;
	pid_t		pids[MAX_PIDS];
	int			pid_count;
	int			any_int;
}	t_shell;

t_quote_type	set_in_quotes(char c, t_quote_type current_quote);
int				check_quotes(const char *input);
int				get_end_of_quotes(const char *str, int start);
int				get_end_of_argument(const char *str, int start);
int				get_next_index(const char *str, int start);

void			init_shell(t_shell *shell, char **envp);
void			cleanup_shell(t_shell *shell, t_command *commands,
					t_token *tokens, char *user_input);

void			setup_signals(void);
void			setup_child_signals(void);
void			handle_signals(int signo);
void			wait_and_check_status(t_shell *shell, int *status);
void			update_exit_status(t_shell *shell, int status);

char			*read_user_input(t_shell *shell);
void			process_input(t_shell *shell);
char			*get_prompt(t_shell *shell);

t_token			*tokenize_input(const char *input, t_shell *shell);
t_command		*organize_commands(t_token *tokens, t_shell *shell);
int				process_special_op(const char *input, int i,
					t_token **tokens, t_shell *shell);
int				process_heredoc_delim(const char *input, int i,
					t_token **tokens, t_shell *shell);
int				process_word(const char *input, int i,
					t_token **tokens, t_shell *shell);
t_token			*token_new(char *value, t_token_type type, t_quote_type quote);
void			token_add(t_token **list, t_token *new_token);
void			token_free(t_token **list);
t_token			*get_last_token(t_token *tokens);

int				validate_token_sequence(t_token *tokens);
int				validate_commands(t_command *commands);

char			*replace_env_variables(const char *input, t_shell *shell);
char			*replace_variable(const char *input, int *i,
					char *result, t_shell *shell);
char			*process_quotes_and_expansion(const char *input,
					t_shell *shell);
char			*append_char(char *result, char c);

void			add_argument_to_command(t_command *command,
					const char *argument, t_shell *shell);
int				handle_single_token(t_token **tokens, t_command **cur,
					t_command **cmds, t_shell *shell);
int				handle_pipe_token(t_token **tokens, t_command **cur,
					t_command **cmds, t_shell *shell);
int				handle_word_token(t_command *cur, t_token *tok,
					t_shell *shell, t_command *cmds);
t_command		*init_new_command(t_shell *shell);
void			free_commands(t_command *commands);

void			execute_commands(t_shell *shell, t_command *commands);
void			execute_single_command(t_shell *shell, t_command *cmd,
					int *prev_pipe_fd);
void			handle_critical_builtin(t_shell *shell, t_command *cmd,
					int pipe_fd[2], int *prev_pipe_fd);
void			handle_forked_command(t_shell *shell, t_command *cmd,
					int pipe_fd[2], int *prev_pipe_fd);
void			wait_for_children(t_shell *shell);

void			execute_child_process(t_shell *shell, t_command *cmd,
					int pipe_fd[2], int prev_pipe_fd);
void			setup_child_pgid(void);
void			execute_child_command(t_command *cmd, t_shell *shell);
void			handle_parent_process(t_shell *shell, t_command *cmd,
					pid_t pid, int *pipe_info);

int				handle_redirections(t_command *cmd);
int				setup_input_redirection(t_command *cmd);
int				setup_output_redirection(t_command *cmd);
void			setup_parent_redirections(t_command *cmd, int pipe_fd[2],
					int prev_pipe_fd);
void			restore_parent_redirections(int saved_stdin, int saved_stdout);
void			update_prev_pipe_fd(t_command *cmd, int pipe_fd[2],
					int *prev_pipe_fd);
void			setup_child_redirections(t_command *cmd, int pipe_fd[2],
					int prev_pipe_fd);
void			close_parent_pipes(t_command *cmd, int pipe_fd[2],
					int *prev_pipe_fd);

int				handle_heredoc(t_command *cmd, t_shell *shell);
void			heredoc_child(t_command *cmd, t_shell *shell,
					int *pipe_fd, int expansion);
int				heredoc_parent(t_command *cmd, t_shell *shell,
					int *pipe_fd, pid_t pid);
int				detect_delimiter_and_strip_quotes(char **del_ptr);
int				setup_heredoc_process(t_command *cmd, int pipe_fd[2],
					int expansion, t_shell *shell);

char			*resolve_command_path(char *command, t_env_var *env,
					t_shell *shell);
char			*check_direct_path(char *command);
char			*find_in_path(char *command, t_env_var *env, t_shell *shell);
char			*try_paths(char *command, char **paths, t_shell *shell);

t_env_var		*copy_env(char **envp, t_shell *shell);
t_env_var		*env_new_var(char *name, char *value, t_shell *shell);
void			env_add_var(t_env_var **env, t_env_var *new_var);
t_env_var		*env_find_var(t_env_var *env, const char *name);
int				count_env_vars(t_env_var *env);
char			**env_to_array(t_env_var *env, t_shell *shell);
void			free_env(t_env_var *env);
void			free_env_array(char **env_array);
void			print_env(t_env_var *env);

int				builtin_env(t_env_var *env);
int				builtin_export(char **args, t_env_var **env, t_shell *shell);
int				builtin_unset(char **args, t_env_var **env);
int				builtin_cd(t_command *cmd, t_shell *shell);
int				builtin_pwd(t_shell *shell);
int				builtin_exit(char **args, t_shell *shell);
void			builtin_echo(char **args, t_shell *shell);
void			print_echo_args(char **args, int i,
					int enable_escape, t_shell *shell);

int				is_builtin(const char *cmd);
int				is_critical_builtin(const char *cmd);
int				is_valid_identifier(const char *str);
char			*get_env_value(t_env_var *env, const char *key);
int				change_directory(const char *path, t_shell *shell);
char			*build_logical_path(const char *old_pwd, const char *arg);
void			update_env_vars(t_env_var **env,
					const char *new_logical_path, t_shell *shell);
void			unset_env_var(t_env_var **env, char *name);
void			set_env_var(t_env_var **env, char *name, char *value,
					t_shell *shell);
int				execute_builtin(t_command *cmd, t_shell *shell);

char			*ft_strjoin_three(const char *s1, const char *s2,
					const char *s3, t_shell *shell);
char			*ft_strjoin_multiple(t_shell *shell, int count, ...);
int				array_len(char **array);
void			free_array(char **array);
void			*safe_malloc(size_t size, t_shell *shell);
char			*safe_strdup(const char *s, t_shell *shell);
char			*safe_substr(const char *s, unsigned int start,
					size_t len, t_shell *shell);
void			*safe_realloc(void *ptr, size_t old_size,
					size_t new_size, t_shell *shell);
void			print_error(const char *cmd, const char *msg);

#endif