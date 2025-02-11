/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:30:51 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/11 08:15:37 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_echo(char **argv)
{
	int	i;
	int	j;
	int	n;

	n = 0;
	j = 1;
	while (argv[j] && argv[j][0] == '-')
	{
		i = 1;
		while (argv[j][i] == 'n')
			i++;
		if (argv[j][i] != '\0')
			break ;
		j++;
	}
	n = j - 1;
	while (argv[j])
	{
		ft_putstr_fd(argv[j++], 1);
		if (argv[j])
			ft_putchar_fd(' ', 1);
	}
	if (!n)
		ft_putstr_fd("\n", 1);
	exit(0);
}
