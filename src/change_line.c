/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:33:33 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/22 20:34:06 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_merge(char *s1, char *s2, int free_s1, int free_s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (free_s1)
		free(s1);
	if (free_s2)
		free(s2);
	if (!tmp)
		ft_exit((char *[]){"exit", "1", NULL}, NULL);
	return (tmp);
}

void	remove_from_line(char *line, int i, int j)
{
	int	m;
	int	len;

	m = 0;
	if (!line || !line[i])
		return ;
	len = ft_strlen(line);
	while (i + m + j < len && line[i + m + j])
	{
		line[i + m] = line[i + m + j];
		m++;
	}
	while (i + m < len)
	{
		line[i + m] = '\0';
		m++;
	}
}
