/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 04:41:37 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/16 05:52:54 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*rev_strdup(char *s, int j)
{
	char	*res;
	int		i;

	res = (char *)malloc(sizeof(char) * (j + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < j)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	free(s);
	return (res);
}

char	*expand(char *line, t_minishell vars)
{
	int	j;
	int	q_flag;
	int	q2_flag;

	j = 0;
	q_flag = -1;
	q2_flag = 1;
	while (line[j])
	{
		if (line[j] == '"' && q_flag == -1)
		{
			q2_flag *= -1;
			j++;
		}
		if (line[j] == '\'' && q2_flag == 1)
		{
			q_flag *= -1;
			j++;
		}
		if (line[j] == '$' && q_flag == -1)
			line = get_variable(vars.env, line, &j, vars.exit_status);
		if (j++ >= (int)ft_strlen(line))
			break ;
	}
	return (line);
}
