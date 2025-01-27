/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 21:42:07 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/26 21:57:12 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv)
{
	int	i;
	int	fd;
	char *line;

	i = 1;
	while (i < argc)
	{
		fd = open(argv[i], O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		i++;
	}
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	return 0;
}
