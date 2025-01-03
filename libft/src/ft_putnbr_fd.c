/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:37:27 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/25 21:08:49 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"
#include "../inc/ft_printf.h"

int	ft_putnbr_fd(long n, int fd)
{
	long int	m;
	int			i;
	char		r;

	m = n;
	i = 1;
	if (m < 0)
	{
		write (fd, "-", 1);
		m *= -1;
		i++;
	}
	if (m > 9)
		i += ft_putnbr_fd(m / 10, fd);
	r = (m % 10) + '0';
	write (fd, &r, 1);
	return (i);
}

int	ft_putnbr_print(long n, t_printf *res, int *i)
{
	long	m;
	char	r;

	m = n;
	if (!res->str)
		return (0);
	if (m < 0)
	{
		*i += write_char_printf('-', res->str);
		m *= -1;
	}
	if (m > 9)
		ft_putnbr_print(m / 10, res, i);
	r = (m % 10) + '0';
	*i += write_char_printf(r, res->str + *i);
	return (*i + 1);
}
