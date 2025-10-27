/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:21:03 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/08 10:54:30 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_str;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	sub_str = (char *)malloc((len + 1) * sizeof(char));
	if (!sub_str)
		return (NULL);
	ft_strlcpy(sub_str, s + start, len + 1);
	return (sub_str);
}

/*
int	main(void)
{
	const char *source ="Ceci est un exemple de sous-chaîne.";
	unsigned int start = 8;  
	size_t len = 11;         	
	char *sub_string = ft_substr(source, start, len);
	if (sub_string == NULL)
	{
		printf("Erreur d'allocation de mémoire.\n");
		return (1);
	}

    printf("Chaîne source : %s\n", source);
    printf("Sous-chaîne : %s\n", sub_string);

    free(sub_string);
    return (0);
}*/
