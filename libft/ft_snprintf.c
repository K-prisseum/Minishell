/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:41:53 by sruzzi            #+#    #+#             */
/*   Updated: 2023/12/02 10:14:07 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	args;
	int		ret;

	va_start(args, format);
	ret = vsnprintf(str, size, format, args);
	va_end(args);
	return (ret);
}
