/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/09 22:16:48 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

int	main(void)
{
	char *line;

	line = NULL;
	printf("%saa\n", line);
	line = readline(NULL);
	// line = get_next_line(0);
	printf("%saa\n", line);
	return 0;
}
