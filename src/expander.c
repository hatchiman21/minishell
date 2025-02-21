/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yousef <yousef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 04:41:37 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/21 16:12:21 by yousef           ###   ########.fr       */
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

int	expander_helper(char *line, int q_flag, int j)
{
	if (!line[j + 1])
		return (0);
	return (line[j] == '$' && line[j + 1] && line[j + 1] != ' '
		&& q_flag == -1 && line[j + 1] != '\'' && line[j + 1] != '|');
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
		if (expander_helper(line, q_flag, j))
			line = get_variable(vars.env, line, &j, vars.exit_status);
		if (j++ >= (int)ft_strlen(line))
			break ;
	}
	return (line);
}
