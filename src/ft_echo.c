/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:30:51 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/24 19:31:01 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	my_echo(char **argv)
{
	int	i;
	int	argc;

	argc = 0;
	if (!argv)
		exit(0);
	while (argv[argc] && argv[argc][0] != '|')
		argc++;
	if (argv[1] && !ft_strncmp(argv[1], "-n", 3))
		i = 2;
	else
		i = 1;
	while (argv[i] && i < argc)
		ft_putstr_fd(argv[i++], 1);
	if (argv[1] && ft_strncmp(argv[1], "-n", 3))
		ft_putchar_fd('\n', 1);
	exit(0);
}
