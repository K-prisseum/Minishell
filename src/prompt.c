/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:29:56 by sruzzi            #+#    #+#             */
/*   Updated: 2024/08/07 15:30:04 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*get_prompt(t_shell *shell)
{
	const char	*color;
	const char	*base_prompt;
	char		*final_prompt;

	color = "\001" GREEN "\002";
	base_prompt = "minishell> ";
	final_prompt = ft_strjoin_three(color, base_prompt,
			"\001" COLOR_RESET "\002", shell);
	if (!final_prompt)
	{
		print_error("get_prompt", "Failed to allocate memory for prompt");
		return (NULL);
	}
	return (final_prompt);
}
