/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qauts_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:56:50 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/15 21:09:34 by aatieh           ###   ########.fr       */
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
	int	q_flag;
	int	counter;

	i = 0;
	q_flag = 0;
	counter = 0;
	while (line[i])
	{
		if ((line[i] == '\'' && q_flag == 1) || (line[i] == '"' && q_flag == 2))
			q_flag = 0;
		else if (line[i] == '\'' && q_flag != 2)
			q_flag = 1;
		else if (line[i] == '"' && !q_flag)
			q_flag = 2;
		if ((line[i] == '\'' && q_flag != 2) || (line[i] == '"' && q_flag != 1))
			counter--;
		counter++;
		i++;
	}
	return (dup_without_qoutes(line, counter));
}

void	remove_all_qoutes(t_minishell *vars)
{
	int			i;
	t_redirect	*red;
	char		*tmp;

	i = 0;
	red = vars->redirections;
	while (i < vars->argc)
	{
		if (vars->argv[i])
		{
			tmp = rm_qoutes(vars->argv[i]);
			free(vars->argv[i]);
			vars->argv[i] = tmp;
		}
		i++;
	}
	while (red)
	{
		tmp = rm_qoutes(red->redirection);
		free(red->redirection);
		red->redirection = tmp;
		red = red->next;
	}
}
