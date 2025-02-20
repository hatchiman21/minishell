/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:37:38 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/20 22:00:14 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	ft_putstr_fd(char *s, int fd)
{
	int	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	write (fd, s, len);
	return (len);
}
