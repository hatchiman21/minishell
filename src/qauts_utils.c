/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qauts_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:56:50 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/22 02:41:15 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*dup_without_qoutes(char *s, int counter)
{
	int		i;
	int		j;
	char	*res;
	int		q_flag;

	res = (char *)malloc(sizeof(char) * (counter + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	j = 0;
	q_flag = 0;
	while (j < counter)
	{
		if ((s[i] == '\'' && q_flag == 1) || (s[i] == '"' && q_flag == 2))
			q_flag = 0;
		else if (s[i] == '\'' && q_flag != 2)
			q_flag = 1;
		else if (s[i] == '"' && !q_flag)
			q_flag = 2;
		if (!((s[i] == '\'' && q_flag != 2) || (s[i] == '"' && q_flag != 1)))
			res[j++] = s[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

char	*rm_qoutes(char *line)
{
	int	i;
	int	j;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			j = i;
			skip_qouted_line(line, &i);
			remove_from_line(line, i, 1);
			remove_from_line(line, j, 1);
			i -= 2;
		}
		i++;
	}
	return (line);
}

void	remove_all_qoutes(t_minishell *vars)
{
	int	i;

	i = 0;
	while (i < vars->argc)
	{
		if (vars->argv[i])
			vars->argv[i] = rm_qoutes(vars->argv[i]);
		i++;
	}
}
