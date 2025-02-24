/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:30:51 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/24 03:18:51 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	handle_n_option(char **argv)
{
	int	i;
	int	j;

	j = 1;
	while (argv[j] && argv[j][0] == '-')
	{
		i = 1;
		while (argv[j][i] == 'n')
			i++;
		if (argv[j][i] != '\0' || i == 1)
			break ;
		j++;
	}
	return (j);
}

int	ft_echo(char **argv)
{
	int	j;
	int	n;

	n = 0;
	j = handle_n_option(argv);
	n = j - 1;
	while (argv[j])
	{
		if (argv[j][0])
			ft_putstr_fd(argv[j], 1);
		j++;
		if (argv[j])
			ft_putchar_fd(' ', 1);
	}
	if (!n)
		ft_putstr_fd("\n", 1);
	return (0);
}
