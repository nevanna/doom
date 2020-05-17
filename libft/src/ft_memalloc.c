/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htillman <htillman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:24:28 by htillman          #+#    #+#             */
/*   Updated: 2019/04/10 22:27:05 by htillman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void *buf;
	if (size == 70*8)
		ft_putendl("memalloc");
	buf = malloc(size);
	if (!buf)
		return (NULL);
	ft_bzero(buf, size);
	return (buf);
}
