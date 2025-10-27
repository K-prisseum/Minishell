/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:44:05 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 16:10:45 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*replace_env_variables(const char *input, t_shell *shell)
{
	char	*result;
	int		i;

	if (!input)
		return (NULL);
	result = safe_strdup("", shell);
	if (!result)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && (ft_isalnum(input[i + 1])
				|| input[i + 1] == '_' || input[i + 1] == '?'))
			result = replace_variable(input, &i, result, shell);
		else
			result = append_char(result, input[i++]);
		if (!result)
			return (NULL);
	}
	return (result);
}


char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_strjoin(result, temp);
	free(result);
	if (!new_result)
	{
		printf("[DEBUG] append_char: Failed to append character\n");
		return (NULL);
	}
	return (new_result);
}

static char	*get_var_value(const char *input, int *i, t_shell *shell)
{
	char	*var_value;
	char	*var_name;
	int		var_start;

	(*i)++;
	if (input[*i] == '?')
	{
		var_value = ft_itoa(shell->exit_stat);
		(*i)++;
		return (var_value);
	}
	var_start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	if (*i == var_start)
		return (NULL);
	var_name = ft_substr(input, var_start, *i - var_start);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(shell->env, var_name);
	free(var_name);
	return (var_value);
}

char	*replace_variable(const char *input, int *i,
	char *result, t_shell *shell)
{
	char	*var_value;
	char	*temp;

	var_value = get_var_value(input, i, shell);
	if (!var_value)
		var_value = safe_strdup("", shell);
	if (!var_value)
		return (NULL);
	temp = ft_strjoin(result, var_value);
	free(var_value);
	if (!temp)
		return (NULL);
	free(result);
	return (temp);
}
